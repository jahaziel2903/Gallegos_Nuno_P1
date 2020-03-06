/*
 * GPIO.c
 *
 *  Created on: 22 feb. 2020
 *      Author: usuario
 */


#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"



uint8_t g_flag_port_C = FALSE;
uint8_t g_flag_port_A = FALSE;

uint8_t g_flag_port_D = FALSE;


/*Pointers to function (callbacks)**/
static void (*gpio_C_callback)(void) = 0;
static void (*gpio_A_callback)(void) = 0;

static void (*gpio_D_callback)(void) = 0;


static gpio_interrupt_flags_t g_intr_status_flag = {0};


void GPIO_callback_init(gpio_port_name_t port_name,void (*handler)(void))
{
	/*If there is a callback im GPIOA, me make a callback to handler**/
	if(GPIO_A == port_name)
	{
		gpio_A_callback = handler;
	}
	else if(GPIO_C == port_name) /*GPIOC makes callback to handler**/
	{
		gpio_C_callback = handler;
	}
	else
		gpio_D_callback = handler;
}


void PORTC_IRQHandler(void)
{
	/*Makes callback**/
	printf("Entro");
	if(gpio_C_callback)
		gpio_C_callback();
	/*There is an interrupt**/
	g_flag_port_C = TRUE;
	/*Cleans interrupt**/
	GPIO_clear_interrupt(GPIO_C);

}

void PORTD_IRQHandler(void)
{
	/*Makes callback**/
	printf("Entro");
	if(gpio_D_callback)
		gpio_D_callback();
	/*There is an interrupt**/
	g_flag_port_D = TRUE;
	/*Cleans interrupt**/
	GPIO_clear_interrupt(GPIO_D);

}
void PORTA_IRQHandler(void)
{
	printf("Entro");
	/*Makes callback**/
	if(gpio_A_callback)
		gpio_A_callback();
	/*There is an interrupt**/
	g_flag_port_A = TRUE;
	/*Cleans interrupt**/
	GPIO_clear_interrupt(GPIO_A);

}

void GPIO_clear_irq_status(gpio_port_name_t gpio)
{
	/*Cleans flag from interrupt GPIOA**/
	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = FALSE;
	}
	else/*Cleans flag from interrupt GPIOC**/
	{
		g_intr_status_flag.flag_port_c = FALSE;
	}
}

uint8_t GPIO_get_irq_status(gpio_port_name_t gpio)
{
	uint8_t status = 0;
	/*Gets status from flag**/
	if(GPIO_A == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	else
	{
		status = g_intr_status_flag.flag_port_c;
	}

	return(status);
}




void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}


uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pin_control_register)
{
	switch(port_name)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pin_control_register;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pin_control_register;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pin_control_register;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pin_control_register;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pin_control_register;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}//end function

void GPIO_data_direction_port(gpio_port_name_t port_name, uint32_t direction)
{
	switch(port_name)/** Selecting the port*/
		{
		case GPIO_A:/** GPIO A is selected*/
			GPIOA->PDDR = ~(direction);
			break;
		case GPIO_B:/**	 GPIO B is selected*/
			GPIOB->PDDR = ~(direction);
			break;
		case GPIO_C:/** GPIO C is selected*/
			GPIOC->PDDR = ~(direction);
			break;
		case GPIO_D:/** GPIO D is selected*/
			GPIOD->PDDR = ~(direction);
			break;
		case GPIO_E:/** GPIO E is selected*/
			GPIOE->PDDR = ~(direction);
		default:/** Port does not exist*/
		break;
		}// end switch
}// end function

void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state, uint8_t pin)
{
	switch(port_name)
	{
		case GPIO_A: /** GPIO A is selected*/
			GPIOA->PDDR &= ~(TRUE << pin);
			GPIOA->PDDR |= state << pin;
			break;

		case GPIO_B: /** GPIO B is selected*/
			GPIOB->PDDR &= ~(TRUE << pin);
			GPIOB->PDDR |= state << pin;
			break;

		case GPIO_C: /** GPIO C is selected*/
			GPIOC->PDDR &= ~(TRUE << pin);
			GPIOC->PDDR |= state << pin;
			break;

		case GPIO_D: /** GPIO D is selected*/
			GPIOD->PDDR &= ~(TRUE << pin);
			GPIOD->PDDR |= state << pin;
			break;

		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PDDR &= ~(TRUE << pin);
			GPIOE->PDDR |= state << pin;
			break;

		default:
			break;
	}// end switch
} //end function

uint32_t GPIO_read_port(gpio_port_name_t port_name){

	uint32_t data;
	switch(port_name)/** Selecting the port*/
			{
			case GPIO_A: /** GPIO A is selected*/
				data = GPIOA->PDIR;
				break;
			case GPIO_B: /** GPIO B is selected*/
				data = GPIOB->PDIR;
				break;
			case GPIO_C: /** GPIO C is selected*/
				data = GPIOC->PDIR;
				break;
			case GPIO_D: /** GPIO D is selected*/
				data = GPIOD->PDIR;
				break;
			case GPIO_E: /** GPIO E is selected*/
				data = GPIOE->PDIR;
			default: /** Port does not exist*/

			break;
			} // end switch
	/** Return data read from the port*/
	return data;
} // end function

uint8_t GPIO_read_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /** Selecting the port*/
	{
		case GPIO_A: /** GPIO A is selected*/
			return((GPIOA->PDIR >> pin) & 0x01);
			break;
		case GPIO_B: /** GPIO B is selected*/
			return((GPIOB->PDIR >> pin) & 0x01);
			break;
		case GPIO_C: /** GPIO C is selected*/
			return((GPIOC->PDIR >> pin) & 0x01);
			break;
		case GPIO_D: /** GPIO D is selected*/
			return((GPIOD->PDIR >> pin) & 0x01);
			break;
		case GPIO_E: /** GPIO E is selected*/
			return((GPIOE->PDIR >> pin) & 0x01);
			break;
		default: /**If doesn't exist the option*/
			return(FALSE);
			break;
	} //end switch
} //end function

void GPIO_write_port(gpio_port_name_t port_name, uint32_t data)
{
	switch(port_name) /** Selecting the port*/
				{
				case GPIO_A: /** GPIO A is selected*/
				 GPIOA->PSOR = data;
					break;
				case GPIO_B: /** GPIO B is selected*/
					 GPIOB->PSOR = data;
					break;
				case GPIO_C: /** GPIO C is selected*/
					 GPIOC->PSOR = data;
					break;
				case GPIO_D: /** GPIO D is selected*/
					 GPIOD->PSOR = data;
					break;
				case GPIO_E: /** GPIO E is selected*/
					 GPIOE->PSOR = data;
				default: /**Port does not exist*/
				break;
				} //end switch
} //end function

void GPIO_set_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /** Selecting the port*/
	{
		case GPIO_A: /** GPIO A is selected*/
			GPIOA->PSOR |= (TRUE << pin);
			break;
		case GPIO_B: /** GPIO B is selected*/
			GPIOB->PSOR |= (TRUE << pin);
			break;
		case GPIO_C: /** GPIO C is selected*/
			GPIOC->PSOR |= (TRUE << pin);
			break;
		case GPIO_D: /** GPIO D is selected*/
			GPIOD->PSOR |= (TRUE << pin);
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PSOR |= (TRUE << pin);
			break;
		default: /**Port does not exist*/

		break;
	} //end switch
} //end function

void GPIO_clear_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /** Selecting the port*/
	{
		case GPIO_A: /** GPIO A is selected*/
			GPIOA->PCOR |= (TRUE << pin);
			break;
		case GPIO_B: /** GPIO B is selected*/
			GPIOB->PCOR |= (TRUE << pin);
			break;
		case GPIO_C: /** GPIO C is selected*/
			GPIOC->PCOR |= (TRUE << pin);
			break;
		case GPIO_D: /** GPIO D is selected*/
			GPIOD->PCOR |= (TRUE << pin);
			break;
		case GPIO_E: /** GPIO E is selected*/
			GPIOE->PCOR |= (TRUE << pin);
			break;
		default: /**Port does not exist*/

		break;
	} //end switch
} //end function

void GPIO_toogle_pin(gpio_port_name_t port_name, uint8_t pin)
{
	switch(port_name) /** Selecting the port*/
				{
				case GPIO_A: /** GPIO A is selected*/
					GPIOA->PTOR |= (TRUE << pin);
					break;
				case GPIO_B: /** GPIO B is selected*/
					GPIOB->PTOR |= (TRUE << pin);
					break;
				case GPIO_C: /** GPIO C is selected*/
					GPIOC->PTOR |= (TRUE << pin);
					break;
				case GPIO_D: /** GPIO D is selected*/
					GPIOD->PTOR |= (TRUE << pin);
					break;
				case GPIO_E: /** GPIO E is selected*/
					GPIOE->PTOR |= (TRUE << pin);
					break;
				default: /**Port does not exist*/

				break;
				} //end switch
} //end function



void GPIO_toogle_data(gpio_port_name_t port_name,  uint32_t data)
{
	switch(port_name) /** Selecting the port*/
				{
				case GPIO_A: /** GPIO A is selected*/
					GPIOA->PTOR = data;
					break;
				case GPIO_B: /** GPIO B is selected*/
					GPIOB->PTOR = data;
					break;
				case GPIO_C: /** GPIO C is selected*/
					GPIOC->PTOR = data;
					break;
				case GPIO_D: /** GPIO D is selected*/
					GPIOD->PTOR = data;
					break;
				case GPIO_E: /** GPIO E is selected*/
					 GPIOE->PTOR = data;
					 break;
				default: /**Port does not exist*/

				break;
				} //end switch
} //end function
