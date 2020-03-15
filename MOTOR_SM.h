/*
 * MOTOR_SM.h
 *
 *  Created on: 7 mar 2020
 *      Author: Jose Isaac Gallegos
 */

#ifndef MOTOR_SM_H_
#define MOTOR_SM_H_

#include "stdint.h"
#include "MK64F12.h"
#include "bits.h"
#include "PIT.h"

#define MOTOR_ON 	1
#define MOTOR_OFF 	0

#define ONE_SEC 	(1.0F)
#define THREE_SEC 	(3.0F)
#define FOUR_SEC 	(4.0F)

#define MOTOR_PIT	PIT_2

typedef enum{
	TURN_ON_1,
	TURN_OFF_1,
	TURN_ON_2,
	TURN_OFF_2,
}motor_state_name_seq_1;

typedef enum{
	TURN_ON,
	TURN_OFF
}motor_state_name_seq_2;

typedef enum{
	SM_OFF,
	SEQ_1,
	SEQ_2
}motor_seq_t;

typedef struct
{
	uint8_t			out;
	My_float_pit_t 	delay;
	uint8_t			next;
}motor_state_t;

typedef struct
{
	uint8_t 				current_state;
	const uint8_t 			initial_state;
	const motor_state_t* 	states;
}motor_sm_t;

void MOTOR_SM_set_seq(motor_seq_t motor_seq);

motor_seq_t MOTOR_SM_get_seq(void);

void MOTOR_SM_reset_seq(motor_seq_t motor_seq);

void MOTOR_SM_pit_callback(void);

void MOTOR_SM_sw2_callback(void);

#endif /* MOTOR_SM_H_ */
