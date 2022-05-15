add_executable(pico_msp main.cpp)

# Link with the pico stdlib
target_link_libraries(pico_msp pico_stdlib hardware_pwm)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(pico_msp)