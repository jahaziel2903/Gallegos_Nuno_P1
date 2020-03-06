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



#define SYSTEM_CLOCK (21000000U)
#define DELAY1 (1.0F)
#define DELAY3 (3.0F)
#define DELAY4 (4.0F)



#define MODE_0 0
#define MODE_1 1
#define MODE_2 2


static uint8_t g_flag_motor = FALSE;



void motor_init()
{
	/*Data pin available**/
	gpio_pin_control_register_t PCR_GPIO_motor_pin = GPIO_MUX1;
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_12, &PCR_GPIO_motor_pin);
	GPIO_data_direction_pin(GPIO_C,GPIO_OUTPUT, bit_12);

	// PIT initialization
	PIT_clock_gating(); /** Enable clock for PIT */
	PIT_enable(); /** Enable PIT with no freeze on debug mode */

	//NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	//NVIC_global_enable_interrupts;

	/** Callback for PIT */
	//PIT_callback_init(PIT_0, choose_sequence);
}


void Motor_on(uint8_t g_flag_motor, uint32_t sw2_value)
{
	if (TRUE == g_flag_motor)
	{
		switch(sw2_value)
		{

		case MODE_1:
			Motor_one_second();
			Motor_off_one_second();
			Motor_three_second();
			Motor_off_three_second();
			break;
		case MODE_2:
			Motor_four_second();
			Motor_off_four_second();
			break;
		default:
			Motor_off();
			break;
		}
	}
	Motor_off(); //aseguramos que el motor al salir de la funcion siempre se apague
}


void Motor_one_second(void)
{
	GPIO_toogle_pin(GPIO_C, bit_12);
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY1);
	//delay(delay);
}


void Motor_three_second(void)
{
	GPIO_toogle_pin(GPIO_C, bit_12);
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY3);
	/*delay(delay);
	delay(delay);
	delay(delay);*/
}

void Motor_four_second(void)
{
	GPIO_toogle_pin(GPIO_C, bit_12);
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY4);
	/*delay(delay);
	delay(delay);
	delay(delay);
	delay(delay);*/
}



void Motor_off_one_second(void)
{
	Motor_off();
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY1);
	//delay(delay);
}


void Motor_off_three_second(void)
{
	Motor_off();
	PIT_delay(PIT_1, SYSTEM_CLOCK, DELAY3);
	/*delay(delay);
	delay(delay);
	delay(delay);*/
}


void Motor_off_four_second(void)
{
	Motor_off();
	PIT_delay(PIT_0, SYSTEM_CLOCK, DELAY4);
	/*delay(delay);
	delay(delay);
	delay(delay);
	delay(delay);*/
}

void Motor_off(void)
{
	if(TRUE == g_flag_motor)
	{
		GPIO_toogle_pin(GPIO_C, bit_16);
	}
}


