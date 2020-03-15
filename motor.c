/*
 * motor.c
 *
 *  Created on: 4 mar. 2020
 *      Author: usuario
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "motor.h"

/*Data pin available**/
gpio_pin_control_register_t PCR_GPIO_motor_pin = GPIO_MUX1;

void MOTOR_init()
{
	GPIO_clock_gating(PORT_MOTOR);
	GPIO_pin_control_register(PORT_MOTOR, PIN_MOTOR, &PCR_GPIO_motor_pin);
	GPIO_data_direction_pin(PORT_MOTOR,GPIO_OUTPUT, PIN_MOTOR);
}

void MOTOR_on(void)
{
	GPIO_set_pin(PORT_MOTOR, PIN_MOTOR);
}

void MOTOR_off(void)
{
	GPIO_clear_pin(PORT_MOTOR, PIN_MOTOR);
}


