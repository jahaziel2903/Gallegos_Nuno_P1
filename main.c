#include "rgb.h"
#include "GPIO.h"
#include "bits.h"
#include "NVIC.h"
#include "PIT.h"
#include "leds.h"
#include "keyboard.h"
#include "MK64F12.h"


#define SYSTEM_CLOCK (21000000U)
#define DELAY (1)


int main(void){
	KEYBOARD_init();
	led_init();
	motor_init();




	while(1){

	}
	return 0;
}
