/*
 * Library: DS1307
 *
 * Description: Short library for RTC module DS1307.
 * 
 * For the correct work of this library,
 * it is need the library for time management on Arduino - "Clock.h".
 *
 * Author: Vereshchynskyi Nazar
 * Email: verechnazar12@gmail.com
 * Version: 1.0.0
 * Date: 08.04.2024
 */

#ifndef _DS1307_H
#define _DS1307_H

#include <Arduino.h>
#include "Clock.h"
#include <Wire.h>

/* --- device address & memory address --- */
#define DS1307_DEVICE_ADDRESS 0x68
#define DS1307_REGISTER_SECOND 0x00
#define DS1307_REGISTER_MINUTE 0x01
#define DS1307_REGISTER_HOUR 0x02
#define DS1307_REGISTER_DAY 0x03
#define DS1307_REGISTER_DATE 0x04
#define DS1307_REGISTER_MONTH 0x05
#define DS1307_REGISTER_YEAR 0x06
#define DS1307_REGISTER_CONTROL 0x07

/* --- main class --- */
class DS1307 {
public:
	DS1307();

	uint8_t begin();
	uint8_t status();

	uint8_t setTime(TimeT);
	TimeT getTime();

private:
	uint8_t read_data(uint8_t, uint8_t*, uint8_t);
	uint8_t read_u8(uint8_t);

	uint8_t write_data(uint8_t, uint8_t*, uint8_t);
	uint8_t write_u8(uint8_t, uint8_t);

	uint8_t dec2bcd(uint8_t);
	uint8_t bcd2dec(uint8_t);

};

#endif