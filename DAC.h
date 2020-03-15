/*
 * ADC.h
 *
 *  Created on: 3 mar 2020
 *      Author: Jose Isaac Gallegos
 */

#ifndef DAC_H_
#define DAC_H_

#include "stdint.h"

#define DAC_0_CLOCK		0x00001000
#define DAC_1_CLOCK		0x00002000

#define DAC_BUFF_WATERMARK_FLAG		0x04
#define DAC_TOP_POSITION_FLAG		0x02
#define DAC_BOTTOM_POSITION_FLAG	0x02

#define DAC_ENABLE					0x80
#define DAC_REFERENCE				0x40
#define DAC_TRIGGER					0x20
#define DAC_SOFTWARE_TRIGGER		0x10
#define DAC_LOW_POWER				0x08
#define DAC_BUFF_WATERMARK_INTR		0x04
#define DAC_POINTER_TOP_INTR		0x02
#define DAC_POINTER_BOTTOM_INTR		0x01

#define DAC_DMA						0x80
#define DAC_BUFF_WATERMARK_1_WORD	0x00
#define DAC_BUFF_WATERMARK_2_WORD	0x08
#define DAC_BUFF_WATERMARK_3_WORD	0x10
#define DAC_BUFF_WATERMARK_4_WORD	0x18
#define DAC_BUFF_NORMAL_MODE		0x00
#define DAC_BUFF_SWING_MODE			0x02
#define DAC_BUFF_SCAN_MODE 			0x04
#define DAC_BUFF_RESERVED			0x06
#define DAC_BUFF					0x01

#define DAC_BUFF_POINTER			0xF0
#define DAC_BUFF_UP_LIMIT			0x0F

#define DATA_LOW(A)		(uint8_t)A
#define DATA_HIGH(A)	(uint8_t)(A>>8)

#define SQUARE_DELAY	(0.1F)
#define SINUSOIDAL_DELAY	(0.002F)
#define TRIANGULAR_DELAY	(0.002F)

#define FIRST_SAMPLE 	0
#define LAST_SAMPLE 	99
#define DAC_ALL_ON		0xFFFF

/*! These constants are used to select an specific DAC in the different API functions*/
typedef enum{DAC_0, /*!< Definition to select DAC 0 */
			 DAC_1, /*!< Definition to select DAC 1 */
			} dac_name_t;

/*! These constants are used to select an specific register from DAC in the different API functions*/
typedef enum{REG_0, /*!< Definition to select register 0 */
			 REG_1, /*!< Definition to select register 1 */
			 REG_2, /*!< Definition to select register 2 */
			 REG_3, /*!< Definition to select register 3 */
			 REG_4, /*!< Definition to select register 4 */
			 REG_5, /*!< Definition to select register 5 */
			 REG_6, /*!< Definition to select register 6 */
			 REG_7, /*!< Definition to select register 7 */
			 REG_8, /*!< Definition to select register 8 */
			 REG_9, /*!< Definition to select register 9 */
			 REG_10, /*!< Definition to select register 10 */
			 REG_11, /*!< Definition to select register 11 */
			 REG_12, /*!< Definition to select register 12 */
			 REG_13, /*!< Definition to select register 13 */
			 REG_14, /*!< Definition to select register 14 */
			 REG_15, /*!< Definition to select register 15 */
			} dac_register_t;

typedef enum{SQUARE_MODE,
			SINUSOIDAL_MODE,
			TRIANGULAR_MODE,
			INITIAL_MODE
			}dac_signal_mode_t;

typedef const uint8_t dac_control_register_t;

void DAC_clock_gating(dac_name_t dac_name);

void DAC_enable(dac_name_t dac_name);

void DAC_disable(dac_name_t dac_name);

void DAC_set_register(dac_name_t dac_name, dac_register_t dac_register, uint16_t data);

void DAC_set_control_0(dac_name_t dac_name, dac_control_register_t* dac_control_register);

void DAC_set_control_1(dac_name_t dac_name, dac_control_register_t* dac_control_register);

void DAC_set_buffer_upper_limit(dac_name_t dac_name, uint8_t buff_up_limit);

uint8_t DAC_buffer_read_pointer(dac_name_t dac_name);

void DAC_set_pit_intr(dac_signal_mode_t signal_mode_t);

void DAC_pit_callback(void);

void DAC_next_mode(void);

void DAC_sw3_callback(void);

#endif /* DAC_H_ */
