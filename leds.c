
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
#include "PIT.h"
#include "NVIC.h"

#define SYSTEM_CLOCK (21000000U)
#define DELAY1 (0.5F)



static uint8_t g_pin_led = FALSE;
static uint8_t g_port_led = FALSE;

/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_11_mot1 = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_10_mot2 = GPIO_MUX1;


/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_3_start = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_2_error = GPIO_MUX1;

/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_5_cuadrada = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOC_7_senoidal = GPIO_MUX1;


 uint8_t led_on_off_counter= FALSE;

void one_second_blink(void)
{
	static uint8_t led = TRUE;
	GPIO_toogle_pin(g_port_led, g_pin_led);
	led_on_off_counter++;

	if(led + 4 ==led_on_off_counter )
	{
		PIT_timer_disable(PIT_0);
		led_on_off_counter = FALSE;
		led = FALSE;
		GPIO_set_pin(g_port_led, g_pin_led);

	}
	else
	{
		PIT_delay( PIT_0,  SYSTEM_CLOCK ,DELAY1);
	}
}

void start_off()
{

}

void one_second_blink_error(void)
{
	//static uint8_t led = TRUE;
	GPIO_toogle_pin(g_port_led, g_pin_led);
	led_on_off_counter++;

	if(4 ==led_on_off_counter )
	{
		PIT_timer_disable(PIT_1);
		led_on_off_counter = FALSE;
	}
	else
	{
		PIT_delay( PIT_1,  SYSTEM_CLOCK ,DELAY1);
	}
}

void led_init()
{
	GPIO_clock_gating(GPIO_C); /** Port B clock configuration */
	GPIO_clock_gating(GPIO_B); /** Port B clock configuration */
	GPIO_clock_gating(GPIO_A); /** Port B clock configuration */


	GPIO_pin_control_register(GPIO_C, bit_11, &PCR_GPIOC_11_mot1); /** Selects alternative function 1 (GPIO)GPIOB pin 21 */
	GPIO_pin_control_register(GPIO_C, bit_10, &PCR_GPIOC_10_mot2); /** Selects alternative function 1 (GPIO) GPIOE pin 26*/
	GPIO_pin_control_register(GPIO_C, bit_3, &PCR_GPIOC_3_start);/** Selects alternative function 1 (GPIO) GPIOB pin 22 */
	GPIO_pin_control_register(GPIO_C, bit_2, &PCR_GPIOC_2_error);/** Selects alternative function 1 (GPIO) GPIOC pin 2 */
	GPIO_pin_control_register(GPIO_C, bit_7, &PCR_GPIOC_7_senoidal);/** Selects alternative function 1 (GPIO) GPIOC pin 7 */
	GPIO_pin_control_register(GPIO_C, bit_5, &PCR_GPIOC_5_cuadrada);/** Selects alternative function 1 (GPIO) GPIOC pin 5 */

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
	//cuadrada
	GPIO_set_pin(GPIO_C, bit_5);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_5);
	//senoidal
	GPIO_set_pin(GPIO_C, bit_7);
	GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, bit_7);
	//todos cad uno tiene su color*/
	initRGB();

	// PIT initialization
	PIT_clock_gating(); /** Enable clock for PIT */
	PIT_enable(); /** Enable PIT with no freeze on debug mode */

	PIT_callback_init(PIT_0, one_second_blink);
	PIT_callback_init(PIT_1, one_second_blink_error);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_3);

	NVIC_global_enable_interrupts;
}

void motor_seq1_led_on()
{
	GPIO_clear_pin(GPIO_C, bit_11);
}
void motor_seq1_led_off()
{
	GPIO_set_pin(GPIO_C, bit_11);
}

void motor_seq2_led_on()
{
	GPIO_clear_pin(GPIO_C, bit_10);
}
void motor_seq2_led_off()
{
	GPIO_set_pin(GPIO_C, bit_10);
}

void start_led_on()
{
	//led_on_off = FALSE;
	g_port_led = GPIO_C;
	g_pin_led = bit_3;
	one_second_blink();
}

void error_led_on()
{
	g_port_led = GPIO_C;
	g_pin_led = bit_2;
	one_second_blink_error();
}
void error_led_off()
{
	GPIO_set_pin(GPIO_C, bit_2);
}
void start_led_off()
{
	GPIO_set_pin(GPIO_C, bit_3);
}

void sine_led_on()
{
	clear_all();
	red_led_on();
	GPIO_clear_pin(GPIO_C, bit_7);
	GPIO_set_pin(GPIO_C, bit_5);
}

void square_led_on()
{
	clear_all();
	blue_led_on();
	GPIO_clear_pin(GPIO_C, bit_5);
	GPIO_set_pin(GPIO_C, bit_7);
}

void triangle_led_on()
{
	clear_all();
	green_led_on();
	GPIO_set_pin(GPIO_C, bit_5);
	GPIO_set_pin(GPIO_C, bit_7);
}

void all_RGB_signals_off()
{
	clear_all();
}

