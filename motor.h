/*
 * motor.h
 *
 *  Created on: 4 mar. 2020
 *      Author: usuario
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#define JAZ_ON

#include "stdint.h"
#include "MK64F12.h"
#include "Bits.h"

#define PORT_MOTOR	GPIO_C
#define PIN_MOTOR	bit_17
void MOTOR_init(void);

void MOTOR_on(void);

void MOTOR_off(void);

#endif /* MOTOR_H_ */
