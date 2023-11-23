/*
 * hdc1080.h
 *
 *  Created on: Oct 12, 2023
 *      Author: Patsaoglou P.
 */

#ifndef INC_HDC1080_H_
#define INC_HDC1080_H_

#include "stm32f4xx_hal.h"

#define HDC1080_DEV_ADD (0x40 << 1)

// Page 14 - Datasheet
#define HDC1080_TEMP_REG 0x00		//Temp Reg Address
#define HDC1080_HUMID_REG 0x01		//Humid Reg Address
#define HDC1080_CONFIG_REG 0x02		//Sensor Configuration Reg Address

typedef struct{
	I2C_HandleTypeDef *hi2c;
}HDC1080;

typedef uint8_t th_err;

enum{
	TH_ERR,
	TH_OK
};

th_err hdc1080_init(HDC1080 *hdc1080, I2C_HandleTypeDef *hi2c);
float hdc1080_get_temp(HDC1080 *hdc1080);
float hdc1080_get_humid(HDC1080 *hdc1080);

#endif /* INC_HDC1080_H_ */
