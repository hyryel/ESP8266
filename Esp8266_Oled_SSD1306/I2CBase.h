// I2CBase.h
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
	#include "wire.h"
#else
	#include "WProgram.h"
#endif

class I2CBase
{
	public:
		I2CBase(byte address, int16_t clockRateKhz);
		I2CBase(byte address);
		void Setup(uint8_t SDA, uint8_t SCL);

	protected:
		void WriteToRegister(byte reg, byte* values, size_t valLength);
		void WriteToRegister(byte reg, byte value);
		int ReadFromRegister(byte reg, byte* readBuffer, size_t buffLength);
		byte ReadFromRegister(byte reg);
		virtual void CustomSetup() {};
	private:
		byte _address;
		byte _clockRate;
		uint8_t _sda, _scl;
		bool _initialized = false;
		void SetClockRate();
};


