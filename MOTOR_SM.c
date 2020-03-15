/*
 * MOTOR_SM.c
 *
 *  Created on: 7 mar 2020
 *      Author: Jose Isaac Gallegos
 */

#include "MOTOR_SM.h"
#include "PIT.h"
#include "motor.h"
#include "leds.h"

static motor_seq_t g_current_seq = SM_OFF;

const motor_state_t motor_state_seq_1[] =
		{
			{MOTOR_ON, ONE_SEC, TURN_OFF_1},
			{MOTOR_OFF, ONE_SEC, TURN_ON_2},
			{MOTOR_ON, THREE_SEC, TURN_OFF_2},
			{MOTOR_OFF, ONE_SEC, TURN_ON_1}
		};

const motor_state_t motor_state_seq_2[] =
			{
				{MOTOR_ON, FOUR_SEC, TURN_OFF},
				{MOTOR_OFF, FOUR_SEC, TURN_ON}
			};

static motor_sm_t motor_sm_seq_1=
		{	TURN_ON_1,
			TURN_ON_1,
			motor_state_seq_1
		};

static motor_sm_t motor_sm_seq_2=
		{	TURN_ON,
			TURN_ON,
			motor_state_seq_2
		};

motor_sm_t* g_motor_seq[3]= {0, &motor_sm_seq_1, &motor_sm_seq_2};

void MOTOR_SM_set_seq(motor_seq_t motor_seq)
{
	g_current_seq = motor_seq;
}

motor_seq_t MOTOR_SM_get_seq(void)
{
	return g_current_seq;
}

void MOTOR_SM_reset_seq(motor_seq_t motor_seq)
{
	if(SM_OFF != motor_seq)
	{
		g_motor_seq[motor_seq]->current_state = g_motor_seq[motor_seq]->initial_state;
	}
}

void MOTOR_SM_pit_callback(void)
{
	uint8_t output;
	uint8_t current_state;
	uint8_t next_state;
	My_float_pit_t delay;

	if(SM_OFF != g_current_seq)
	{
		current_state = g_motor_seq[g_current_seq]->current_state;
		delay = g_motor_seq[g_current_seq]->states[current_state].delay;
		PIT_timer_disable(MOTOR_PIT);
		PIT_delay(MOTOR_PIT,SYSTEM_CLOCK,delay);

		output = g_motor_seq[g_current_seq]->states[current_state].out;
		next_state = g_motor_seq[g_current_seq]->states[current_state].next;

		if(MOTOR_ON == output)
		{
			MOTOR_on();
		}
		else
		{
			MOTOR_off();
		}
		g_motor_seq[g_current_seq]->current_state = next_state;
	}
	else
	{
		PIT_timer_disable(MOTOR_PIT);
		MOTOR_off();
	}
}

void MOTOR_SM_sw2_callback(void)
{
	if(SEQ_2 == g_current_seq)
	{
		g_current_seq = SM_OFF;
	}
	else
	{
		g_current_seq++;
	}

	MOTOR_SM_reset_seq(g_current_seq);
	MOTOR_SM_set_seq(g_current_seq);
	MOTOR_SM_pit_callback();

	switch(g_current_seq)
	{
	case SM_OFF:
		motor_seq1_led_off();
		motor_seq2_led_off();
		break;
	case SEQ_1:
		motor_seq1_led_on();
		motor_seq2_led_off();
		break;
	case SEQ_2:
		motor_seq1_led_off();
		motor_seq2_led_on();
		break;
	}

}
