add_executable(Blinky main.c )

target_link_libraries(Blinky PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        KineticLib
)

pico_set_program_name(Blinky "Blinky")
pico_set_program_version(Blinky "0.3")

pico_enable_stdio_uart(Blinky 0)
pico_enable_stdio_usb(Blinky 1)

target_include_directories(Blinky PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(Blinky)