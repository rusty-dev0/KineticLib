add_executable(EncoderCalibration main.c )

target_link_libraries(EncoderCalibration PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        KineticLib
)

pico_set_program_name(EncoderCalibration "EncoderCalibration")
pico_set_program_version(EncoderCalibration "0.1")

pico_enable_stdio_uart(EncoderCalibration 1)
pico_enable_stdio_usb(EncoderCalibration 0)

target_include_directories(EncoderCalibration PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(EncoderCalibration)