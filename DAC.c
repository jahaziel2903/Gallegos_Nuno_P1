/*
 * ADC.c
 *
 *  Created on: 3 mar 2020
 *      Author: Jose Isaac Gallegos
 */
#include <DAC.h>
#include "Bits.h"
#include "MK64F12.h"
#include "PIT.h"
#include "rgb.h"
#include "leds.h"
#include "GPIO.h"

static dac_signal_mode_t g_dac_signal_mode = INITIAL_MODE;

const uint16_t SINUSOIDAL_100_SAMPLES[] =
	{0x800, 0x880, 0x900, 0x97F, 0x9FD, 0xA78, 0xAF1, 0xB67, 0xBDA, 0xC49,
	0xCB3, 0xD19, 0xD79, 0xDD4, 0xE29, 0xE78, 0xEC0, 0xF02, 0xF3C, 0xF6F,
	0xF9B, 0xFBF, 0xFDB, 0xFEF, 0xFFB, 0xFFF, 0xFFB, 0xFEF, 0xFDB, 0xFBF,
	0xF9B, 0xF6F, 0xF3C, 0xF02, 0xEC0, 0xE78, 0xE29, 0xDD4, 0xD79, 0xD19,
	0xCB3, 0xC49, 0xBDA, 0xB67, 0xAF1, 0xA78, 0x9FD, 0x97F, 0x900, 0x880,
	0x800, 0x77F, 0x6FF, 0x680, 0x602, 0x587, 0x50E, 0x498, 0x425, 0x3B6,
	0x34C, 0x2E6, 0x286, 0x22B, 0x1D6, 0x187, 0x13F, 0x0FD, 0x0C3, 0x090,
	0x064, 0x040, 0x024, 0x010, 0x004, 0x000, 0x004, 0x010, 0x024, 0x040,
	0x064, 0x090, 0x0C3, 0x0FD, 0x13F, 0x187, 0x1D6, 0x22B, 0x286, 0x2E6,
	0x34C, 0x3B6, 0x425, 0x498, 0x50E, 0x587, 0x602, 0x680, 0x6FF, 0x77F};

const uint16_t TRIANGULAR_100_SAMPLES[] =
	{0x000, 0x052, 0x0A4, 0x0F6, 0x148, 0x19A, 0x1EB, 0x23D, 0x28F, 0x2E1,
	0x333, 0x385, 0x3D7, 0x429, 0x47B, 0x4CD, 0x51E, 0x570, 0x5C2, 0x614,
	0x666, 0x6B8, 0x70A, 0x75C, 0x7AE, 0x800, 0x851, 0x8A3, 0x8F5, 0x947,
	0x999, 0x9EB, 0xA3D, 0xA8F, 0xAE1, 0xB33, 0xB84, 0xBD6, 0xC28, 0xC7A,
	0xCCC, 0xD1E, 0xD70, 0xDC2, 0xE14, 0xE66, 0xEB7, 0xF09, 0xF5B, 0xFAD,
	0xFFF, 0xFAD, 0xF5B, 0xF09, 0xEB7, 0xE66, 0xE14, 0xDC2, 0xD70, 0xD1E,
	0xCCC, 0xC7A, 0xC28, 0xBD6, 0xB84, 0xB33, 0xAE1, 0xA8F, 0xA3D, 0x9EB,
	0x999, 0x947, 0x8F5, 0x8A3, 0x851, 0x800, 0x7AE, 0x75C, 0x70A, 0x6B8,
	0x666, 0x614, 0x5C2, 0x570, 0x51E, 0x4CD, 0x47B, 0x429, 0x3D7, 0x385,
	0x333, 0x2E1, 0x28F, 0x23D, 0x1EB, 0x19A, 0x148, 0x0F6, 0x0A4, 0x052};

void DAC_clock_gating(dac_name_t dac_name)
{
	switch (dac_name)
	{
	case DAC_0:
		SIM->SCGC2 |= DAC_0_CLOCK;
		break;
	case DAC_1:
		SIM->SCGC2 |= DAC_1_CLOCK;
		break;
	default:
		break;
	}
}

void DAC_enable(dac_name_t dac_name)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->C0 |= DAC_ENABLE;
		break;
	case DAC_1:
		DAC1->C0 |= DAC_ENABLE;
		break;
	default:
		break;
	}
}

void DAC_disable(dac_name_t dac_name)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->C0 &= ~DAC_ENABLE;
		break;
	case DAC_1:
		DAC1->C0 &= ~DAC_ENABLE;
		break;
	default:
		break;
	}
}

void DAC_set_register(dac_name_t dac_name, dac_register_t dac_register, uint16_t data)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->DAT[dac_register].DATL = DATA_LOW(data);
		DAC0->DAT[dac_register].DATH = DATA_HIGH(data);
		break;
	case DAC_1:
		DAC1->DAT[dac_register].DATL = DATA_LOW(data);
		DAC1->DAT[dac_register].DATH = DATA_HIGH(data);
		break;
	default:
		break;
	}
}

void DAC_set_control_0(dac_name_t dac_name, dac_control_register_t* dac_control_register)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->C0 &= DAC_ENABLE;
		DAC0->C0 |= *dac_control_register;
		break;
	case DAC_1:
		DAC1->C0 &= DAC_ENABLE;
		DAC1->C0 |= *dac_control_register;
		break;
	default:
		break;
	}
}

void DAC_set_control_1(dac_name_t dac_name, dac_control_register_t* dac_control_register)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->C1 = *dac_control_register;
		break;
	case DAC_1:
		DAC1->C1 = *dac_control_register;
		break;
	default:
		break;
	}
}

void DAC_set_buffer_upper_limit(dac_name_t dac_name, uint8_t buff_up_limit)
{
	switch (dac_name)
	{
	case DAC_0:
		DAC0->C2 = buff_up_limit;
		break;
	case DAC_1:
		DAC1->C2 = buff_up_limit;
		break;
	default:
		break;
	}
}

uint8_t DAC_buffer_read_pointer(dac_name_t dac_name)
{
	uint8_t buffer_read_pointer = 0;

	switch (dac_name)
	{
	case DAC_0:
		buffer_read_pointer = DAC0->C2;
		break;
	case DAC_1:
		buffer_read_pointer = DAC1->C2;
		break;
	default:
		break;
	}

	return buffer_read_pointer;
}

void DAC_set_pit_intr(dac_signal_mode_t signal_mode_t)
{
	switch(signal_mode_t)
	{
	case SQUARE_MODE:
		PIT_delay(PIT_3, SYSTEM_CLOCK, SQUARE_DELAY);
		g_dac_signal_mode = SQUARE_MODE;
		break;
	case SINUSOIDAL_MODE:
		PIT_delay(PIT_3, SYSTEM_CLOCK, SINUSOIDAL_DELAY);
		g_dac_signal_mode = SINUSOIDAL_MODE;
		break;
	case TRIANGULAR_MODE:
		PIT_delay(PIT_3, SYSTEM_CLOCK, TRIANGULAR_DELAY);
		g_dac_signal_mode = TRIANGULAR_MODE;
		break;
	case INITIAL_MODE:
		PIT_timer_disable(PIT_3);
		DAC_set_register(DAC_0, REG_0, (uint16_t)~DAC_ALL_ON);
		clear_all();
		GPIO_set_pin(GPIO_C, bit_5);
		GPIO_set_pin(GPIO_C, bit_7);
		g_dac_signal_mode = INITIAL_MODE;
	default:
		break;
	}
}


void DAC_pit_callback(void)
{
	static uint8_t counter_sample = FIRST_SAMPLE;
	static uint16_t square_state = DAC_ALL_ON;
	switch(g_dac_signal_mode)
	{
	case SQUARE_MODE:
		DAC_set_register(DAC_0, REG_0, square_state);
		square_state = ~square_state;
		break;
	case SINUSOIDAL_MODE:
		DAC_set_register(DAC_0, REG_0, SINUSOIDAL_100_SAMPLES[counter_sample]);
		counter_sample = (LAST_SAMPLE == counter_sample)?FIRST_SAMPLE:counter_sample + 1;
		break;
	case TRIANGULAR_MODE:
		DAC_set_register(DAC_0, REG_0, TRIANGULAR_100_SAMPLES[counter_sample]);
		counter_sample = (LAST_SAMPLE == counter_sample)?FIRST_SAMPLE:counter_sample + 1;
		break;
	default:
		break;
	}
}

void DAC_next_mode(void)
{
	g_dac_signal_mode ++;
	if(INITIAL_MODE <= g_dac_signal_mode)
	{
		g_dac_signal_mode = SQUARE_MODE;
	}
	DAC_set_pit_intr(g_dac_signal_mode);
}

void DAC_sw3_callback(void)
{
	DAC_next_mode();
	switch(g_dac_signal_mode)
	{
	case SQUARE_MODE:
		square_led_on();
		break;
	case SINUSOIDAL_MODE:
		sine_led_on();
		break;
	case TRIANGULAR_MODE:
		triangle_led_on();
		break;
	default:
		break;
	}
}
