/*
 * rgb.h
 *
 *  Created on: 14 feb. 2020
 *      Author: usuario
 */

// verde, azul, morado rojo amraillo
//blanco si se presiona los dos

#ifndef RGB_H_
#define RGB_H_

#include "stdint.h"
#include "bits.h"

typedef enum {GREEN = 1, BLUE, PURPLE, RED, YELLOW} Color;
typedef enum {sw2 = 0, sw3} Current_switch;

/** Constant that represent the value that turns on RED Led */
#define RGB_RED_ON 0x00400000
/** Constant that represent the value that turns on GREEN Led */
#define RGB_GREEN_ON 0x004000000
/** Constant that represent the value that turns on BLUE Led */
#define RGB_BLUE_ON 0x00200000
/** Constant that represent the value that turns on RED & BLUE Led */
#define RGB_BLUEN_RED_ON 0x00600000

/** Constant that represent the value that turns off RED Led */
#define RGB_RED_OFF 0x00400000
/** Constant that represent the value that turns off GREEN Led */
#define RGB_GREEN_OFF 0x004000000
/** Constant that represent the value that turns off BLUE Led */
#define RGB_BLUE_OFF 0x00200000
/** Constant that represent the value that turns off BLUE & RED Led */
#define RGB_BLUEN_RED_OFF 0x00600000

/** Constant that represent the PIN of RED Led */
#define RED_LED_PIN bit_22
/** Constant that represent the PIN of GREEN Led */
#define GREEN_LED_PIN bit_26
/** Constant that represent the PIN of BLUE Led */
#define BLUE_LED_PIN bit_21

#define Switch2 bit_6
#define Switch3 bit_4

#define SW2_PIN_MASK  0X40
#define SW3_PIN_MASK  0X10

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function initializes the RGB leds, uses functions from the GPIO library, clock gating
 	  \GPIO_pin_control_register, GPIO_write_port, GPIO_data_direction_port
	 \ does not received a parameter
 */

void initRGB();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns on the red led using the GPIO library
	 \ does not received a parameter
 */

void red_led_on();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns on the green led using the GPIO library
	 \ does not received a parameter
 */


void green_led_on();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns on the blue led using the GPIO library
	 \ does not received a parameter
 */

void blue_led_on();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns on the purple led using the GPIO library
	 \ does not received a parameter
 */

void purple_led_on();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns on the yellow led using the GPIO library
	 \ does not received a parameter
 */

void yellow_led_on();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns on the white led using the GPIO library
	 \ does not received a parameter
 */

void white_led_on();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns off the red led using the GPIO library
	 \ does not received a parameter
 */

void red_led_off();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns off the green led using the GPIO library
	 \ does not received a parameter
 */


void green_led_off();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns off the blue led using the GPIO library
	 \ does not received a parameter
 */

void blue_led_off();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns off the purple led using the GPIO library
	 \ does not received a parameter
 */

void purple_led_off();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns off the yellow led using the GPIO library
	 \ does not received a parameter
 */

void yellow_led_off();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

/*!
 	  \This function turns off the white led using the GPIO library
	 \ does not received a parameter
 */

void white_led_off();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function turns off RGB leds using the GPIO library
	 \ does not received a parameter
 */
void clear_all();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function returns the current switch
	 \ does not received a parameter
 */
uint8_t GPIO_get_sw_status(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function changes current switch to sw2
	 \ does not received a parameter
 */
void change_to_sw2(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function changes current switch to sw3
	 \ does not received a parameter
 */
void change_to_sw3(void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	  \This function changes the flow in which leds are turn on
 	   \ does not received a parameter
 */
void Color_change (void);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

#endif /* RGB_H_ */
