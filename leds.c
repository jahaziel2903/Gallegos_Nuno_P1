
/*
 * leds.c
 *
 *  Created on: 4 mar. 2020
 *      Author: usuario
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "rgb.h"


/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_11_mot1 = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_10_mot2 = GPIO_MUX1;


/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_3_start = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_2_error = GPIO_MUX1;


led_init()
{
	GPIO_clock_gating(GPIO_C); /** Port B clock configuration */


	GPIO_pin_control_register(GPIO_C, bit_11, &PCR_GPIOC_11_mot1); /** Selects alternative function 1 (GPIO)GPIOB pin 21 */
	GPIO_pin_control_register(GPIO_C, bit_10, &PCR_GPIOC_10_mot2); /** Selects alternative function 1 (GPIO) GPIOE pin 26*/
	GPIO_pin_control_register(GPIO_C, bit_3, &PCR_GPIOC_3_start);/** Selects alternative function 1 (GPIO) GPIOB pin 22 */
	GPIO_pin_control_register(GPIO_C, bit_2, &PCR_GPIOC_2_error);/** Selects alternative function 1 (GPIO) GPIOB pin 22 */


	//set pin y data direction para cada uno
	//Motor secuencia 1
	GPIO_set_pin(GPIO_C, bit_11);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_11);
	//MOTOR SECUENCIA 2
	GPIO_set_pin(GPIO_C, bit_10);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_10);
	//LED START
	GPIO_set_pin(GPIO_C, bit_3);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_3);
	//LED ERROR
	GPIO_set_pin(GPIO_C, bit_2);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_2);
	//LEDS DE SEÑALES
	initRGB();

	// PIT initialization
	PIT_clock_gating(); /** Enable clock for PIT */
	PIT_enable(); /** Enable PIT with no freeze on debug mode */
}

motor_seq1_led_on()
{
	GPIO_clear_pin(GPIO_C, bit_11);
}
motor_seq1_led_off()
{
	GPIO_set_pin(GPIO_C, bit_12);
}

motor_seq2_led_on()
{
	GPIO_clear_pin(GPIO_C, bit_10);
}
motor_seq2_led_off()
{
	GPIO_set_pin(GPIO_C, bit_10);
}

start_led_on()
{
	GPIO_clear_pin(GPIO_C, bit_3);
}
start_led_off()
{
	GPIO_set_pin(GPIO_C, bit_2);

}

sine_led_on()
{
	red_led_on();
}
sine_led_off()
{
	red_led_off();
}

square_led_on()
{
	blue_led_on();
}
square_led_off()
{
	blue_led_off();
}
triangle_led_on()
{
	green_led_on();
}
triangle_led_off()
{
	green_led_of();
}



