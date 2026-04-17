file(GLOB_RECURSE SOURCES
    "edge-impulse-sdk/classifier/*.cpp"
    "edge-impulse-sdk/classifier/*.cc"
    "edge-impulse-sdk/classifier/*.c"
    "edge-impulse-sdk/dsp/*.cpp"
    "edge-impulse-sdk/dsp/*.cc"
    "edge-impulse-sdk/dsp/*.c"
    "edge-impulse-sdk/porting/espressif/*.cpp"
    "edge-impulse-sdk/porting/espressif/*.cc"
    "edge-impulse-sdk/porting/espressif/*.c"
    "edge-impulse-sdk/porting/espressif/esp-dsp/modules/fft/float/*.c"   # <-- ADD THIS
    "edge-impulse-sdk/tensorflow/*.cpp"
    "edge-impulse-sdk/tensorflow/*.cc"
    "edge-impulse-sdk/tensorflow/*.c"
    "tflite-model/*.cpp"
    "tflite-model/*.cc"
    "tflite-model/*.c"
)

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS
        "."
        "model-parameters"
        "tflite-model"
        "edge-impulse-sdk"
        "edge-impulse-sdk/classifier"
        "edge-impulse-sdk/dsp"
        "edge-impulse-sdk/tensorflow"
        "edge-impulse-sdk/tensorflow/lite"
        "edge-impulse-sdk/tensorflow/lite/micro"
        "edge-impulse-sdk/tensorflow/lite/c"
        "edge-impulse-sdk/porting"
        "edge-impulse-sdk/porting/espressif"
        "edge-impulse-sdk/porting/espressif/ESP-NN/include"
        "edge-impulse-sdk/porting/espressif/esp-dsp/modules/common/include"
        "edge-impulse-sdk/porting/espressif/esp-dsp/modules/fft/include"
        "edge-impulse-sdk/third_party"
        "edge-impulse-sdk/third_party/flatbuffers/include"
        "edge-impulse-sdk/third_party/gemmlowp"
        "edge-impulse-sdk/third_party/ruy"
    PRIV_REQUIRES esp_timer esp-dsp
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    "-Wno-error"
    "-Wno-error=maybe-uninitialized"
    "-Wno-maybe-uninitialized"
)