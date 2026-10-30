add_executable(TreadCarefully main.c )

target_link_libraries(TreadCarefully PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        KineticLib
)

pico_set_program_name(TreadCarefully "TreadCarefully")
pico_set_program_version(TreadCarefully "0.1")

pico_enable_stdio_uart(TreadCarefully 1)
pico_enable_stdio_usb(TreadCarefully 0)

target_include_directories(TreadCarefully PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(TreadCarefully)