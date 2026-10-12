add_executable(MotorDrive main.c )

target_link_libraries(MotorDrive PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        KineticLib
)

pico_set_program_name(MotorDrive "MotorDrive")
pico_set_program_version(MotorDrive "0.1")

pico_enable_stdio_uart(MotorDrive 0)
pico_enable_stdio_usb(MotorDrive 1)

target_include_directories(MotorDrive PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(MotorDrive)