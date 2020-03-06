#include "stdint.h"
#include "GPIO.h"
#include "rgb.h"

/** Configures PortB bit 21 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOB_21 = GPIO_MUX1;
/** Configures PortB bit 22 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOB_22 = GPIO_MUX1;
/** Configures PortE bit 26 as a multiplexer*/
gpio_pin_control_register_t PCR_GPIOE_26 = GPIO_MUX1;



uint8_t g_current_switch = sw3;
Color g_current_led = YELLOW;


void initRGB()
{
	GPIO_clock_gating(GPIO_B); /** Port B clock configuration */
	GPIO_clock_gating(GPIO_E); /**Port E clock configuration */

	GPIO_pin_control_register(GPIO_B, BLUE_LED_PIN, &PCR_GPIOB_21); /** Selects alternative function 1 (GPIO)GPIOB pin 21 */
	GPIO_pin_control_register(GPIO_E, GREEN_LED_PIN, &PCR_GPIOE_26); /** Selects alternative function 1 (GPIO) GPIOE pin 26*/
	GPIO_pin_control_register(GPIO_B, RED_LED_PIN, &PCR_GPIOB_22);/** Selects alternative function 1 (GPIO) GPIOB pin 22 */

	GPIO_write_port(GPIO_B, 0x00600000); /**Writes in  port E, RED and BLUE leds */
	GPIO_data_direction_port(GPIO_B, ~0x00600000); /**GPIOB as output */

	GPIO_write_port(GPIO_E, 0x04000000); /**Writes in  port E  GREEN Led*/
	GPIO_data_direction_port(GPIO_E, ~0x04000000);/**GPIOE as output */
}


void red_led_on()
{
	/**Uses GPIO library, toggles to turn on RED Led */
	GPIO_clear_pin(GPIO_B,  RED_LED_PIN);
}

void green_led_on()
{
	/**Uses GPIO library, to turn on GREEN Led */
	GPIO_clear_pin(GPIO_E,  GREEN_LED_PIN);
}

void blue_led_on()
{
	/**Uses GPIO library, to turn on BLUE Led */
	GPIO_clear_pin(GPIO_B, BLUE_LED_PIN);
}

void purple_led_on()
{
	/**Turns BLUE and RED Led on to get  PURPLE Led */
	blue_led_on();
	red_led_on();
}
void yellow_led_on()
{
	/**Uses GPIO library, toggles to turn on YELLOW Led */
	green_led_on();
	red_led_on();
}
void white_led_on()
{
	/**Uses GPIO library, toggles to turn on WHITE Led */
	green_led_on();
	red_led_on();
	blue_led_on();
}

void red_led_off()
{
	GPIO_set_pin(GPIO_B,  RED_LED_PIN);
}

void green_led_off()
{
	GPIO_set_pin(GPIO_E,  GREEN_LED_PIN);
}
void blue_led_off()
{
	GPIO_set_pin(GPIO_B, BLUE_LED_PIN);
}

void purple_led_off()
{
	blue_led_off();
	red_led_off();

}
void yellow_led_off()
{
	green_led_off();
	red_led_off();
}
void white_led_off()
{
	green_led_off();
	red_led_off();
	blue_led_off();
}

void clear_all()
{
	white_led_off();
}


void change_to_sw2(void)
{
	g_current_switch = sw2;
	//printf("Entro a interrupcion sw2");
}
void change_to_sw3(void)
{
	g_current_switch = sw3;
	//printf("Entro a interrupcion sw3");
}

uint8_t GPIO_get_sw_status(void)
{
	return g_current_switch;
}

void Color_change(void)
{
	/**Function that turns on the color from the corresponding Led*/
	if(sw3 ==g_current_switch )
	{
		switch(g_current_led)
		{
		case GREEN: /**GREEN COLOR*/
			green_led_off();
			blue_led_on();
			g_current_led = BLUE;
			break;
		case BLUE:/**BLUE COLOR*/
			blue_led_off();
			purple_led_on();
			g_current_led = PURPLE;
			break;
		case PURPLE:/**PURPLE COLOR*/
			purple_led_off();
			red_led_on();
			g_current_led = RED;
			break;
		case RED:/**RED COLOR*/
			red_led_off();
			yellow_led_on();
			g_current_led = YELLOW;
			break;
		case YELLOW:/**YELLOW COLOR*/
			yellow_led_off();
			green_led_on();
			g_current_led = GREEN;
			break;
		default: /**NO COLOR*/
			break;
		}
	}
	else
	{
		switch(g_current_led)
		{
		case GREEN: /**GREEN COLOR*/
			green_led_off();
			yellow_led_on();
			g_current_led = YELLOW;
			break;
		case BLUE:/**BLUE COLOR*/
			blue_led_off();
			green_led_on();
			g_current_led = GREEN;
			break;
		case PURPLE:/**PURPLE COLOR*/
			purple_led_off();
			blue_led_on();
			g_current_led = BLUE;
			break;
		case RED:/**RED COLOR*/
			red_led_off();
			purple_led_on();
			g_current_led = PURPLE;
			break;
		case YELLOW:/**YELLOW COLOR*/
			yellow_led_off();
			red_led_on();
			g_current_led = RED;
			break;
		default: /**NO COLOR*/
			break;
		}

	}
}










