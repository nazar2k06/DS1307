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

#include "DS1307.h"

/* --- constuctor --- */
DS1307::DS1307() {
	Wire.begin();
}

/* --- begin --- */
uint8_t DS1307::begin() {
	uint8_t second_byte = 0;
	uint8_t err_code = read_data(DS1307_REGISTER_SECOND, &second_byte, 1);

	if (err_code) {
		return err_code;
	}

	if (second_byte & 0x80) {
		second_byte &= 0x7F;

		if (write_u8(DS1307_REGISTER_SECOND, second_byte)) {
			return 3;
		}
	}

	return 0;
}

/* --- status --- */
uint8_t DS1307::status() {
	uint8_t second_byte = 0;
	uint8_t err_code = read_data(DS1307_REGISTER_SECOND, &second_byte, 1);

	if (err_code) {
		return err_code;
	}

	if (second_byte & 0x80) {
		return 3;
	}

	return 0;
}


/* --- set time --- */
uint8_t DS1307::setTime(TimeT set_time) {
	uint8_t bytes[7];

	bytes[0] = 0x80;
	bytes[1] = dec2bcd(set_time.minute);
	bytes[2] = dec2bcd(set_time.hour);
	bytes[3] = dec2bcd(set_time.weekday);
	bytes[4] = dec2bcd(set_time.day);
	bytes[5] = dec2bcd(set_time.month);
	if (set_time.year >= 2000) {
		bytes[6] = dec2bcd(set_time.year - 2000);
	}

	if (write_data(DS1307_REGISTER_SECOND, bytes, 7)) {
		return 1;
	}
	write_u8(DS1307_REGISTER_SECOND, dec2bcd(set_time.second));

	return 0;
}

/* --- get time --- */
TimeT DS1307::getTime() {
	TimeT read_time;
	uint8_t bytes[7];

	if (read_data(DS1307_REGISTER_SECOND, bytes, 7)) {
		return read_time;
	}

	read_time.second = bcd2dec(bytes[0] & 0x7F);
	read_time.minute = bcd2dec(bytes[1]);
	read_time.hour = bcd2dec(bytes[2] & 0x3F);
	read_time.weekday = bcd2dec(bytes[3]);
	read_time.day = bcd2dec(bytes[4]);
	read_time.month = bcd2dec(bytes[5]);
	read_time.year = bcd2dec(bytes[6]) + 2000;

	return read_time;
}


/* --- read data from device memory --- */
uint8_t DS1307::read_data(uint8_t reg, uint8_t* pointer, uint8_t len) {
	Wire.beginTransmission(DS1307_DEVICE_ADDRESS);
	Wire.write(reg);

	if (Wire.endTransmission()) {
		return 1;
	}

	Wire.requestFrom((int)DS1307_DEVICE_ADDRESS, (int)len);
	if (!Wire.available()) {
		return 2;
	}

	while (Wire.available() && pointer != NULL) {
		*pointer = Wire.read();
		pointer++;
	}

	return 0;
}

/* --- read unsigned byte from register --- */
uint8_t DS1307::read_u8(uint8_t reg) {
	uint8_t data = 0;

	read_data(reg, &data, 1);

	return data;
}


/* --- write data to device memory --- */
uint8_t DS1307::write_data(uint8_t reg, uint8_t* pointer, uint8_t len) {
	Wire.beginTransmission(DS1307_DEVICE_ADDRESS);
	Wire.write(reg);

	for (uint8_t i = 0;i < len;i++) {
		Wire.write(pointer[i]);
	}

	if (Wire.endTransmission()) {
		return 1;
	}

	return 0;
}

/* --- write unsigned byte to register --- */
uint8_t DS1307::write_u8(uint8_t reg, uint8_t data) {
	return write_data(reg, &data, 1);
}


/* --- convert from decimal code to binary decimal code --- */
uint8_t DS1307::dec2bcd(uint8_t num) {
	return ((num / 10 * 16) + (num % 10));
}

/* --- convert from binary decimal code to decimal code --- */
uint8_t DS1307::bcd2dec(uint8_t num) {
	return ((num / 16 * 10) + (num % 16));
}