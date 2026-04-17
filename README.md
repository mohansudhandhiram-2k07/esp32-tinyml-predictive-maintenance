# ESP32 tinyml predictive maintenance system (IIoT Predictive Maintenance)

An end-to-end, scalable Industrial IoT (IIoT) framework designed for real-time anomaly detection and predictive maintenance on edge devices. 

Currently implementing vibration-based mechanical fault classification using TinyML, with an architecture designed to support future multi-sensor fusion.

## System Architecture
Instead of streaming raw data to the cloud for processing, this system performs ultra-low latency inference directly on the edge node. 
* **Data Acquisition:** High-frequency tri-axial vibration data streamed via I2C.
* **DSP & Inference:** On-device feature extraction and quantized neural network execution.
* **Alerting:** Real-time classification of operational states (Normal vs. Anomaly).

## Tech Stack
* **Microcontroller:** ESP32 
* **Sensor:** MPU6050 (6-axis IMU)
* **Framework:** ESP-IDF (C/C++)
* **TinyML:** Edge Impulse SDK
* **RTOS:** FreeRTOS (Task management & IPC)

## Scalability Roadmap
This node is designed to scale beyond single-axis vibration monitoring:
- [x] Phase 1: Vibration anomaly detection (MPU6050)
- [ ] Phase 2: Thermal anomaly monitoring integration
- [ ] Phase 3: MQTT telemetry for fleet-wide dashboarding