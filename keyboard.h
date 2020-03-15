#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include "stdint.h"
#include "NVIC.h"
#include "GPIO.h"
#include "MK64F12.h"
#include "Bits.h"
#include "RGB.h"

#define SIZE 4

typedef enum{
	right, wrong, incomplete }password_status_t;

typedef enum{
	button_1, button_2, button_3, button_A,
	button_4, button_5, button_6, button_B,
	button_7, button_8, button_9, button_C,
	button_asterisk, button_0 , button_hashtag,
	button_D, none}keyboard_button;

typedef enum {
	global_locked, global_unlocked,motor_unlocked, signal_unlocked,
	motor_locked,signal_locked
}status_t;


/* Passwords **/
static keyboard_button master[] = {button_1, button_2, button_3, button_4};
static keyboard_button motor[] = {button_4, button_5, button_6, button_7};
static keyboard_button signal[] = {button_7, button_8, button_9, button_0};

static keyboard_button input[4];


/*Funcion para inicializar teclado**/
void KEYBOARD_init();

/*Funcion para devolver tecla**/
void KEYBOARD_read_button();

/*Funcion para saber que se desbloqueo**/
//password_status_t KEYBOARD_password_status(keyboard_button_t, password_status_t);

password_status_t master_correct (keyboard_button key);
password_status_t signal_correct (keyboard_button key);
password_status_t motor_correct (keyboard_button key);


keyboard_button get_key();

#endif /* KEYBOARD_H_ */
