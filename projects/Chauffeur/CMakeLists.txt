add_executable(Chauffeur main.c )

target_link_libraries(Chauffeur PRIVATE
        pico_stdlib
        pico_cyw43_arch_none
        hardware_uart
        KineticLib
)

pico_set_program_name(Chauffeur "Chauffeur")
pico_set_program_version(Chauffeur "0.1")

pico_enable_stdio_uart(Chauffeur 1)
pico_enable_stdio_usb(Chauffeur 0)

target_include_directories(Chauffeur PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_add_extra_outputs(Chauffeur)