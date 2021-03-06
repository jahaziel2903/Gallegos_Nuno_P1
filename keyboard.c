/*
 * buttonboard.c
 *
 *  Created on: 28 feb. 2020
 *      Author: usuario
 */
#include "keyboard.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "leds.h"
#include "bits.h"
#include "PIT.h"

#define SYSTEM_CLOCK (21000000U)
#define DELAY1 (1.0F)



#include "keyboard.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "leds.h"
#include "bits.h"
#include "PIT.h"

#define SYSTEM_CLOCK (21000000U)
#define DELAY1 (1.0F)



keyboard_button g_button = none;
uint16_t value = FALSE;
uint8_t same = TRUE;

void KEYBOARD_init()
{
	/*Data pin available**/
	gpio_pin_control_register_t PCR_GPIO_DA_pin = GPIO_MUX1 | GPIO_PE |INTR_RISING_EDGE;
	/*Columns as inputs**/
	gpio_pin_control_register_t PCR_GPIO_in_ports = GPIO_MUX1 | GPIO_PE ;

	/*Configuration of the four ports to read the buttonboard matrix*/
	/*Columnns and Data available pin */
	GPIO_clock_gating(GPIO_D);
	GPIO_clock_gating(GPIO_B);

	/*Ports as inputs**/
	GPIO_pin_control_register(GPIO_D, bit_0, &PCR_GPIO_in_ports);
	GPIO_data_direction_pin(GPIO_D,GPIO_INPUT, bit_0);

	GPIO_pin_control_register(GPIO_D, bit_1, &PCR_GPIO_in_ports);
	GPIO_data_direction_pin(GPIO_D,GPIO_INPUT, bit_1);

	GPIO_pin_control_register(GPIO_D, bit_2, &PCR_GPIO_in_ports);
	GPIO_data_direction_pin(GPIO_D,GPIO_INPUT, bit_2);

	GPIO_pin_control_register(GPIO_D, bit_3, &PCR_GPIO_in_ports);
	GPIO_data_direction_pin(GPIO_D,GPIO_INPUT, bit_3);

	/*Data available input**/
	GPIO_pin_control_register(GPIO_B, bit_2, &PCR_GPIO_DA_pin);
	GPIO_data_direction_pin(GPIO_B,GPIO_INPUT, bit_2);

	/*In case there is an interruption we pass to know which button was pressed**/
	GPIO_callback_init(GPIO_B, KEYBOARD_read_button);

	/*We configure interruptions**/
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ, PRIORITY_1);
	NVIC_global_enable_interrupts;

	// PIT initialization
	//PIT_clock_gating(); /** Enable clock for PIT */
	//PIT_enable(); /** Enable PIT with no freeze on debug mode */
}

void KEYBOARD_read_button()
{
	uint16_t value_in_port = GPIO_read_port(GPIO_D);
	printf("%d",value_in_port );
	switch(value_in_port)
	{
	case 0:
		g_button = button_1;
		break;
	case 1:
		g_button = button_2;
		break;
	case 2:
		g_button = button_3;
		break;
	case 3:
		g_button = button_A;
		break;
	case 4:
		g_button = button_4;
		break;
	case 5:
		g_button = button_5;
		break;
	case 6:
		g_button = button_6;
		break;
	case 7:
		g_button = button_B;
		break;
	case 8:
		g_button = button_7;
		break;
	case 9:
		g_button = button_8;
		break;
	case 10:
		g_button = button_9;
		break;
	case 11:
		g_button = button_C;
		break;
	case 12:
		g_button = button_asterisk;
		break;
	case 13:
		g_button = button_0;
		break;
	case 14:
		g_button = button_hashtag;
		break;
	case 15:
		g_button = button_D;
		break;

	default:
		g_button= none;
		break;
	}



}


keyboard_button get_key()
{
	keyboard_button temp = g_button ;
	//printf("%d", temp);
	g_button = none;
	return temp;

}



password_status_t master_correct (keyboard_button key)
{
	static uint8_t introduce = FALSE;
	static uint8_t correct = FALSE;
	if(none != key)
	{
		introduce++;
		if(master[correct] == key)
		{
			correct ++;
		}
	}


	if(introduce == 4)
	{
		if (correct == 4)
		{
			introduce = FALSE;
			correct = FALSE;
			return right;
		}
		else
		{
			introduce = FALSE;
			correct = FALSE;
			return wrong;
		}
	}
	else
	{
		return incomplete;
	}

}

password_status_t motor_correct (keyboard_button key)
{
	static uint8_t introduce = FALSE;
		static uint8_t correct = FALSE;
		if(none != key)
		{
			introduce++;
			if(motor[correct] == key)
			{
				correct ++;
			}
		}


		if(introduce == 4)
		{
			if (correct == 4)
			{
				introduce = FALSE;
				correct = FALSE;
				return right;
			}
			else
			{
				introduce = FALSE;
				correct = FALSE;
				return wrong;
			}
		}
		else
		{
			return incomplete;
		}
}
password_status_t signal_correct (keyboard_button key)
{
	static uint8_t introduce = FALSE;
		static uint8_t correct = FALSE;
		if(none != key)
		{
			introduce++;
			if(signal[correct] == key)
			{
				correct ++;
			}
		}


		if(introduce == 4)
		{
			if (correct == 4)
			{
				introduce = FALSE;
				correct = FALSE;
				return right;
			}
			else
			{
				introduce = FALSE;
				correct = FALSE;
				return wrong;
			}
		}
		else
		{
			return incomplete;
		}
}

/*
void master_led_correct()
{

	start_led_on();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	start_led_off();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	start_led_on();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	start_led_off();

}

void master_led_incorrect()
{

	error_led_on();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	error_led_off();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	error_led_on();
	PIT_delay(PIT_2, SYSTEM_CLOCK, DELAY1);
	error_led_off();

}

 *
uint8_t signal_correct (keyboard_button key)
{
	static uint8_t introduce = FALSE;
	static uint8_t correct = FALSE;

	if(key == master[correct])
	{
		correct ++;
	}

	introduce++;

	if(introduce == 4)
	{
		if (correct == 4)
		{
			return right;
		}
		else
		{
			return wrong;
		}
	}
	else
	{
		return incomplete;
	}
}


uint8_t motor_correct (keyboard_button key)
{
	static uint8_t introduce = FALSE;
	static uint8_t correct = FALSE;

	if(key == master[correct])
	{
		correct ++;
	}

	introduce++;

	if(introduce == 4)
	{
		if (correct == 4)
		{
			return right;
		}
		else
		{
			return wrong;
		}
	}
	else
	{
		return incomplete;
	}
}*/




