add_executable(HelloPi main.c )

target_link_libraries(HelloPi PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        KineticLib
)

pico_set_program_name(HelloPi "HelloPi")
pico_set_program_version(HelloPi "0.1")

pico_enable_stdio_uart(HelloPi 1)
pico_enable_stdio_usb(HelloPi 0)

target_include_directories(HelloPi PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(HelloPi)