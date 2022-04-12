add_executable(
	compass
		compass.c
	lib/fonts.c
	lib/st7735.c
	lib/DEV_Config.c
	lib/ICM20948.c
)

# pull in common dependencies
target_link_libraries(
	compass
	hardware_pwm
	hardware_pio
	hardware_spi
	hardware_i2c
	pico_stdlib
)

# create map/bin/hex file etc.
pico_add_extra_outputs(compass)

# add url via pico_set_program_url
example_auto_set_url(compass)