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

volatile uint32_t g_dummy_read = 0;
static pit_interrupt_flags_t pit_intr_status_flag = {FALSE};

static void (*PIT_0_callback)(void) = 0;
static void (*PIT_1_callback)(void) = 0;
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;


void PIT_delay(PIT_timer_t pit_timer, My_float_pit_t system_clock , My_float_pit_t delay)
{
	uint32_t LDVAL = 0;
	uint32_t pit_frequency = 0;
	pit_frequency = system_clock;
	LDVAL = (uint32_t)(delay*pit_frequency);	//32-bit float assumed
	LDVAL = LDVAL-1;	//Value to load into LDVAL
	/* Number of cycles loaded into the channel that is in use**/
	PIT->CHANNEL[pit_timer].LDVAL = LDVAL;

	PIT_enable_interrupt(pit_timer);
	//* The interrupt for Timer 1 is enabled by setting TCTRL1[TIE].
	//The timer is started by writing 1 to TCTRL1[TEN]*/
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_clock_gating(void)
{
	/*Enalbles PIT clock gating*/
	/*SIM_SCGC6_PIT_MASK = 0x800000U = PIT Pin*/
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(PIT_timer_t pit_timer)
{
	uint8_t flag_interrupt = FALSE;
	/*Return the status of the interrupt flag, created by the programmer*/
	switch(pit_timer)
	{
	case PIT_0:
		flag_interrupt = pit_intr_status_flag.flag_PIT_0;
		break;
	case PIT_1:
		flag_interrupt = pit_intr_status_flag.flag_PIT_1;
		break;
	case PIT_2:
		flag_interrupt = pit_intr_status_flag.flag_PIT_2;
		break;
	case PIT_3:
		flag_interrupt = pit_intr_status_flag.flag_PIT_3;
		break;
	default:
		break;
	}
	return flag_interrupt;
}

void PIT_clear_interrupt_flag(void)
{
	pit_intr_status_flag.flag_PIT_0 = FALSE;
	pit_intr_status_flag.flag_PIT_1 = FALSE;
	pit_intr_status_flag.flag_PIT_2 = FALSE;
	pit_intr_status_flag.flag_PIT_3 = FALSE;
}

void PIT_enable(void)
{
	/*Timer stopped in debug Mode **/
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	/*Clock for standard PIT timers is not disabled.**/
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
}

void PIT_disable(void)
{
	/*Clock for standard PIT timers is disabled.**/
	PIT->MCR &= PIT_MCR_MDIS_MASK;
}

void PIT_timer_enable(PIT_timer_t pit_timer)
{
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_timer_disable(PIT_timer_t pit_timer)
{
	PIT->CHANNEL[pit_timer].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_enable_interrupt(PIT_timer_t pit_timer)
{
	/*Clears the flag**/
	PIT->CHANNEL[pit_timer].TFLG |= PIT_TFLG_TIF_MASK;
	/*Enables the interrupt**/
	PIT->CHANNEL[pit_timer].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void PIT_disable_interrupt(PIT_timer_t pit_timer)
{
	PIT->CHANNEL[pit_timer].TCTRL = MASK_PIT_DISABLE;//disable interuptions
}

void PIT0_IRQHandler(void)
{
	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
	g_dummy_read = PIT->CHANNEL[PIT_0].TCTRL;
	pit_intr_status_flag.flag_PIT_0 = TRUE;

	/*CALLBACK*/
	if(PIT_0_callback)
	{
		PIT_0_callback();
	}
}

void PIT1_IRQHandler(void)
{
	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK;
	g_dummy_read = PIT->CHANNEL[PIT_1].TCTRL;
	pit_intr_status_flag.flag_PIT_1 = TRUE;

	/*CALLBACK*/
	if(PIT_1_callback)
	{
		PIT_1_callback();
	}
}

void PIT2_IRQHandler(void)
{
	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
	g_dummy_read = PIT->CHANNEL[PIT_2].TCTRL;
	pit_intr_status_flag.flag_PIT_2 = TRUE;

	/*CALLBACK*/
	if(PIT_2_callback)
	{
		PIT_2_callback();
	}
}

void PIT3_IRQHandler(void)
{
	PIT->CHANNEL[PIT_3].TFLG |= PIT_TFLG_TIF_MASK;
	g_dummy_read = PIT->CHANNEL[PIT_3].TCTRL;
	pit_intr_status_flag.flag_PIT_3 = TRUE;

	/*CALLBACK*/
	if(PIT_3_callback)
	{
		PIT_3_callback();
	}
}

void PIT_callback_init(PIT_timer_t pit_timer,void (*handler)(void))
{
	switch(pit_timer)
	{
	case PIT_0:
		PIT_0_callback = handler;
		break;
	case PIT_1:
		PIT_1_callback = handler;
		break;
	case PIT_2:
		PIT_2_callback = handler;
		break;
	case PIT_3:
		PIT_3_callback = handler;
		break;
	default:
		break;
	}
}
