// 
// 
// 

#include "I2CBase.h"

void I2CBase::WriteToRegister(byte reg, byte * values, size_t valLength)
{
	if (_initialized)
	{
		//advertise the slave device that we will send something
		Wire.beginTransmission(_address);
		//select the register to write to
		Wire.write(reg);
		//write data to the selected register
		Wire.write(values, valLength);
		Wire.endTransmission();
	}
}

void I2CBase::WriteToRegister(byte reg, byte value)
{
	if (_initialized)
	{
		//advertise the slave device that we will send something
		Wire.beginTransmission(_address);
		//select the register to write to
		Wire.write(reg);
		//write data to the selected register
		Wire.write(value);
		Wire.endTransmission();
	}
}

int I2CBase::ReadFromRegister(byte reg, byte* readBuffer, size_t buffLenght)
{
	if (_initialized)
	{
		//Advertise the slave device that we will need some data
		Wire.beginTransmission(_address);
		//Select the register to read from
		Wire.write(reg);
		//Send the instruction and Instruct the slave that we don't have finished with him
		Wire.endTransmission(false);
		//Ask the slave to read
		Wire.requestFrom(_address, buffLenght);
		int readCount = Wire.readBytes(readBuffer, buffLenght);
		return readCount;
	}
	return 0;
}

I2CBase::I2CBase(byte address, int16_t clockRateKhz)
{
	_address = address;
	_clockRate = clockRateKhz;
}

I2CBase::I2CBase(byte address) : I2CBase(address, 400)
{}

void I2CBase::Setup(uint8_t SDA, uint8_t SCL)
{
	_scl = SCL;
	_sda = SDA;
	Wire.begin(_sda, _scl);
	Wire.setClock(_clockRate);
	_initialized = true;
}

