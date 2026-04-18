#include "driver/i2c.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>


static const char *TAG = "VIBRATION_AI";

// --- I2C CONFIG (Match your physical pins) ---
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000
#define MPU6050_ADDR 0x68
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1 0x6B

void mpu6050_init() {
  i2c_config_t conf = {};
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
  conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  i2c_param_config(I2C_MASTER_NUM, &conf);
  i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

  // Wake up MPU6050
  uint8_t data[] = {MPU6050_PWR_MGMT_1, 0x00};
  i2c_master_write_to_device(I2C_MASTER_NUM, MPU6050_ADDR, data, 2,
                             pdMS_TO_TICKS(100));
}

void get_accel(float *ax, float *ay, float *az) {
  uint8_t reg = MPU6050_ACCEL_XOUT_H;
  uint8_t data[6];
  i2c_master_write_read_device(I2C_MASTER_NUM, MPU6050_ADDR, &reg, 1, data, 6,
                               pdMS_TO_TICKS(100));
  *ax = (float)((int16_t)((data[0] << 8) | data[1]));
  *ay = (float)((int16_t)((data[2] << 8) | data[3]));
  *az = (float)((int16_t)((data[4] << 8) | data[5]));
}

extern "C" void app_main() {
  mpu6050_init();
  ESP_LOGI(TAG, "Edge AI Vibration Monitor Active.");

  while (1) {
    // Edge Impulse uses a specific buffer size based on your 'Window' (1000ms =
    // 300 samples)
    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

    // 1. Data Collection (100Hz Sampling)
    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
      uint64_t start = esp_timer_get_time();
      get_accel(&buffer[ix], &buffer[ix + 1], &buffer[ix + 2]);

      // Maintain 10ms sampling interval
      uint64_t end = esp_timer_get_time();
      int64_t sleep_us = 10000 - (end - start);
      if (sleep_us > 0) {
        esp_rom_delay_us((uint32_t)sleep_us);
      }
    }

    // 2. Wrap buffer for Edge Impulse
    signal_t signal;
    int res = numpy::signal_from_buffer(
        buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

    // 3. Run Inference
    ei_impulse_result_t result = {0};
    EI_IMPULSE_ERROR ei_res = run_classifier(&signal, &result, false);
    if (ei_res != EI_IMPULSE_OK) {
      ESP_LOGE(TAG, "Classifier Error: %d", ei_res);
      continue;
    }

    // 4. Output Logic
    printf("\n--- PREDICTION ---\n");
    printf("Normal: %.2f | Anomaly: %.2f\n", result.classification[1].value,
           result.classification[0].value);

    if (result.classification[0].value > 0.80) {
      ESP_LOGE(TAG, "!!! FAILURE IMMINENT: ANOMALY DETECTED !!!");
    } else {
      ESP_LOGI(TAG, "System Health: OK");
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Short breather before next window
  }
}