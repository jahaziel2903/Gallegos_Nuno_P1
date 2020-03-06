/*
 * PIT.c
 *
 *  Created on: 22 feb. 2020
 *      Author: usuario
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "Bits.h"
#include "PIT.h"

/*An interruption has ocurred*/
static uint8_t flag_interrupt = FALSE;

volatile uint8_t g_dummy_read = 0;

static void (*PIT_0_callback)(void) = 0;
static void (*PIT_1_callback)(void) = 0;
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;

void PIT_callback_init(PIT_timer_t pit_name,void (*handler)(void))
{
	switch(pit_name){
		case PIT_0: /** GPIO A is selected*/
			PIT_0_callback = handler;
			break;
		case PIT_1: /** GPIO B is selected*/
			PIT_1_callback = handler;
			break;
			break;
		case PIT_2: /** GPIO C is selected*/
			PIT_2_callback = handler;
			break;
		case PIT_3: /** GPIO D is selected*/
			PIT_3_callback = handler;
			break;
		default: /** GPIO E is selected*/
			break;
	}
}


void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	My_float_pit_t pit_clk = system_clock;


	/*Value to load into LDVAL*/
	uint32_t trigger = (uint32_t)(delay*pit_clk);
	/* Turn on PIT MDIS = 0, FRZ = 0**/
	/* PIT Timers enabled, continue to run in Debug Time**/

	/* Number of cycles loaded into the channel that is in use**/
	PIT->CHANNEL[pit_timer].LDVAL =(trigger);
	PIT_enable_interrupt(pit_timer);
	 //* The interrupt for Timer 1 is enabled by setting TCTRL1[TIE].
	/*The timer is started by writing 1 to TCTRL1[TEN]**/
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_clock_gating(void)
{
	/*Enalbles PIT clock gating*/
	/*SIM_SCGC6_PIT_MASK = 0x800000U = PIT Pin*/
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(void)
{
	/*Return the status of the interrupt flag, created by the programmer**/
	return flag_interrupt;
}

void PIT_clear_interrupt_flag(void)
{
	flag_interrupt = FALSE;
}
void PIT_enable(void)
{
	/*Clock for standard PIT timers is not disabled.**/
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	/*Timer stopped in debug Mode **/
	PIT->MCR  |= PIT_MCR_FRZ_MASK;

}

void PIT_enable_interrupt(PIT_timer_t pit)
{
	/*Clears the flag*/
		PIT->CHANNEL[pit].TFLG 	|= PIT_TFLG_TIF_MASK;
		/*Enables the interrupt*/
		PIT->CHANNEL[pit].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void PIT0_IRQHandler(void)
{
	if(PIT_0_callback)
			PIT_0_callback();
	PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	flag_interrupt |= 0x01;
	g_dummy_read = PIT->CHANNEL[0].TCTRL;
}

static void PIT1_IRQHandler(void)
{
	if(PIT_1_callback)
			PIT_1_callback();

	PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
	flag_interrupt |= 0x02;
	g_dummy_read = PIT->CHANNEL[1].TCTRL;
}

static void PIT2_IRQHandler(void)
{
	if(PIT_2_callback)
			PIT_2_callback();
	PIT->CHANNEL[2].TFLG |= PIT_TFLG_TIF_MASK;
	flag_interrupt |= 0x04;
	g_dummy_read = PIT->CHANNEL[2].TCTRL;
}

static void PIT3_IRQHandler(void)
{
	if(PIT_3_callback)
			PIT_3_callback();
	PIT->CHANNEL[3].TFLG |= PIT_TFLG_TIF_MASK;
	flag_interrupt |= 0x08;
	g_dummy_read = PIT->CHANNEL[3].TCTRL;
}

