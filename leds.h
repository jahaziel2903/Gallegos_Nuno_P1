/*
 * leds.h
 *
 *  Created on: 4 mar. 2020
 *      Author: usuario
 */

#ifndef LEDS_H_
#define LEDS_H_

void led_init();

void motor_seq1_led_on();
void motor_seq1_led_off();

void motor_seq2_led_on();
void motor_seq2_led_off();

void start_led_on();
void start_led_off();

void error_led_on();
void error_led_off();

void sine_led_on();
void sine_led_off();

void square_led_on();
void square_led_off();

void triangle_led_on();
void triangle_led_off();
#endif /* LEDS_H_ */
