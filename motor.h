/*
 * motor.h
 *
 *  Created on: 4 mar. 2020
 *      Author: usuario
 */

#ifndef MOTOR_H_
#define MOTOR_H_


#include "stdint.h"
#include "NVIC.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "Bits.h"
#include "PIT.h"



void Motor_on(uint8_t g_flag_motor, uint32_t sw2_value);



void Motor_one_second(void);
void Motor_three_second(void);
void Motor_four_second(void);


void Motor_off_one_second(void);
void Motor_off_three_second(void);
void Motor_off_four_second(void);


void Motor_off(void);


#endif /* MOTOR_H_ */
