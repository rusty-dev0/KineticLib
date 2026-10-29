add_library(KineticLib STATIC
    src/Motor.c
    src/Encoder.c #in development
)

target_include_directories(KineticLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(KineticLib PRIVATE
    pico_stdlib
    hardware_pwm
    hardware_gpio
    hardware_irq
)