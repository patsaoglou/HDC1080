/*
 * HDC1080.c
 *
 *  Created on: Oct 12, 2023
 *      Author: Patsaoglou P.
 *
 */

#include "hdc1080.h"

/*
 *	Performs software reset of the sensor with memory write at REG ADDRESS=0x02 with data
 *	0x8000 (Bit[15] = 1) and sets Temperature/Humidity resolution to 14bit with memory write
 *	at REG ADDRESS=0x02 with data 0x0000 (Bit[10] = 0 (Temp Bit), Bit[9:8] = 00 (Humid Bits)).
 *	For more configuration options refer the datasheet at page 15.
 *
 *	NOTE: All data bytes are trasmitted MSB first (Page 10)
 */
th_err hdc1080_init(HDC1080 *hdc1080, I2C_HandleTypeDef *hi2c){
	hdc1080->hi2c = hi2c;
	uint8_t data[2];

	data[0] = 0x80;
	data[1] = 0x00;

	//	Performing software reset
	if (HAL_I2C_Mem_Write(hdc1080->hi2c, HDC1080_DEV_ADD, HDC1080_CONFIG_REG, 1, data, 2, 100) != HAL_OK){
		return TH_ERR;
	}

	HAL_Delay(20);

	data[0] = 0x00;
	data[1] = 0x00;
	if (HAL_I2C_Mem_Write(hdc1080->hi2c, HDC1080_DEV_ADD, HDC1080_CONFIG_REG, 1, data, 2, 100) != HAL_OK){
		return TH_ERR;
	}

	return TH_OK;
}

/*
 *	Performs a pointer write transaction by setting address pointer to 0x00 to trigger a
 *	temperature measurement, waits for conversion, reads the temperature data from temperature register address (Page 12)
 *	and calculates actual temperature values using the provided formula (Page 14).
 */
float hdc1080_get_temp(HDC1080 *hdc1080){
	float temp;
	uint8_t data[2];
	uint16_t temp_data;

	data[0] = HDC1080_TEMP_REG;

	HAL_I2C_Master_Transmit(hdc1080->hi2c, HDC1080_DEV_ADD, data, 1, 100);

	HAL_Delay(20);

	HAL_I2C_Master_Receive(hdc1080->hi2c, HDC1080_DEV_ADD, data, 2, 100);

	temp_data = data[0] << 8;
	temp_data = temp_data | data[1];

	temp = (temp_data / 65536.0) * 165 -40;

	return temp;
}

/*
 *	Performs a pointer write transaction by setting address pointer to 0x01 to trigger a
 *	humidity measurement, waits for conversion, reads the humidity data from humidity register
 *	address (Page 12) and calculates actual humidity values using the provided formula (Page 14).
 */
float hdc1080_get_humid(HDC1080 *hdc1080){
	float humid;
	uint8_t data[2];
	uint16_t humid_data;

	data[0] = HDC1080_HUMID_REG;

	HAL_I2C_Master_Transmit(hdc1080->hi2c, HDC1080_DEV_ADD, data, 1, 100);

	HAL_Delay(20);

	HAL_I2C_Master_Receive(hdc1080->hi2c, HDC1080_DEV_ADD, data, 2, 100);

	humid_data = data[0] << 8;
	humid_data = humid_data | data[1];

	humid = (humid_data / 65536.0) * 100;

	return humid;
}
