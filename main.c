#include "rgb.h"
#include "GPIO.h"
#include "bits.h"
#include "NVIC.h"
#include "PIT.h"
#include "motor.h"
#include "MOTOR_SM.h"
#include "leds.h"
#include "keyboard.h"
#include "DAC.h"
#include "MK64F12.h"

int main(void){

	/** Configures switch 2 as a pull up Multiplexer */
	gpio_pin_control_register_t PCR_SW2 = GPIO_MUX1 |GPIO_PE | GPIO_PS | GPIO_PS | INTR_FALLING_EDGE;
	/** Configures switch 3 as a pull up Multiplexer */
	gpio_pin_control_register_t PCR_SW3 = GPIO_MUX1 |GPIO_PE | GPIO_PS | GPIO_PS | INTR_FALLING_EDGE;

	/**Initialization of all things related to switches*/
	GPIO_clock_gating(GPIO_C); /** Port C clock configuration */
	GPIO_clock_gating(GPIO_A); /** Port A clock configuration */
	/** Selects alternative function 1 (GPIO), selects and enables pull-up resistor switch 2*/
	GPIO_pin_control_register(GPIO_C, Switch2, &PCR_SW2);
	/** Selects alternative function 1 (GPIO), selects and enables pull-up resistor switch 3 */
	GPIO_pin_control_register(GPIO_A, Switch3, &PCR_SW3);
	GPIO_data_direction_pin(GPIO_C, GPIO_INPUT, Switch2); //setup sw2
	GPIO_data_direction_pin(GPIO_A, GPIO_INPUT, Switch3); //setup sw3

	dac_control_register_t DAC0_C0 = DAC_REFERENCE;

	status_t 		g_status;
	keyboard_button key_input;
	uint8_t 		motor_is_lock;
	uint8_t 		signal_is_lock;
	g_status = 			global_locked;
	key_input = 		none;
	motor_is_lock = 	TRUE;
	signal_is_lock = 	TRUE;

	KEYBOARD_init();
	led_init();
	MOTOR_init();
	password_status_t pass = FALSE;

	/**Enables and sets SW2 interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	/**Enables and sets SW3 interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_5);
	/**Enables and sets PIT0 interrupt and its priority*/
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_2);

	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_4);
	DAC_clock_gating(DAC_0);
	DAC_set_control_0(DAC_0, &DAC0_C0);
	DAC_enable(DAC_0);

	PIT_callback_init(PIT_3, DAC_pit_callback);



	PIT_callback_init(MOTOR_PIT, MOTOR_SM_pit_callback);

	while(1)
	{
		//get_key();
		switch(g_status)
		{
		case global_locked:
			pass = master_correct(get_key());
			if(right == pass)
			{
				start_led_on();
				g_status = global_unlocked;
			}
			else if(wrong == pass)
			{
				error_led_on();
			}
			break;

		case global_unlocked:
			key_input = get_key();
			if(button_A == key_input)
			{
				if(motor_is_lock)
				{
					g_status = motor_unlocked;
				}
				else
				{
					g_status = motor_locked;
				}
			}
			else if(button_B == key_input)
			{
				if(signal_is_lock)
				{
					g_status = signal_unlocked;
				}
				else
				{
					g_status = signal_locked;
				}
			}
			break;

		case motor_unlocked:
			pass = motor_correct(get_key());
			if(right == pass)
			{
				start_led_on();
				GPIO_callback_init(GPIO_C,MOTOR_SM_sw2_callback);
				motor_is_lock = FALSE;
				g_status = global_unlocked;
			}
			else if(wrong == pass)
			{
				error_led_on();
			}
			break;

		case signal_unlocked:
			pass = signal_correct(get_key());
			if(right == pass)
			{
				start_led_on();
				GPIO_callback_init(GPIO_A,DAC_sw3_callback);
				signal_is_lock = FALSE;
				g_status = global_unlocked;
			}
			else if(wrong == pass)
			{
				error_led_on();
			}
			break;

		case motor_locked:
			pass = motor_correct(get_key());
			if(right == pass)
			{
				start_led_on();
				GPIO_callback_init(GPIO_C,0);
				MOTOR_SM_set_seq(SEQ_2);
				MOTOR_SM_sw2_callback();
				motor_is_lock = TRUE;
				g_status = global_unlocked;
			}
			else if(wrong == pass)
			{
				error_led_on();
			}
			break;

		case signal_locked:
			pass = signal_correct(get_key());
			if(right == pass)
			{
				start_led_on();
				GPIO_callback_init(GPIO_A,0);
				DAC_set_pit_intr(INITIAL_MODE);
				signal_is_lock = TRUE;
				g_status = global_unlocked;
			}
			else if(wrong == pass)
			{
				error_led_on();
			}
			break;

		default:
			g_status = global_locked;
			break;
		}
	}
	return 0;
}
