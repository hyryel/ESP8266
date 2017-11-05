// 
// 
// 

#include "I2CBase.h"
//Write values to the register
//reg : Address of the register to write to
//values : the values to write to
//valLength : the count of bytes to write
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
//Write value to the register
//reg : address of the register to write to
//value : the value to write
void I2CBase::WriteToRegister(byte reg, byte value)
{
	if (_initialized)
	{
		SetClockRate();
		//advertise the slave device that we will send something
		Wire.beginTransmission(_address);
		//select the register to write to
		Wire.write(reg);
		//write data to the selected register
		Wire.write(value);
		Wire.endTransmission();
	}
}
//Read buffer from register.
//reg : address of register to read
//readBuffer : a byte* to receive the value read
//bufferLength : the count of byte to read
//return the number of bytes read
int I2CBase::ReadFromRegister(byte reg, byte* readBuffer, size_t buffLength)
{
	if (_initialized)
	{
		SetClockRate();
		//Advertise the slave device that we will need some data
		Wire.beginTransmission(_address);
		//Select the register to read from
		Wire.write(reg);
		//Send the instruction and Instruct the slave that we don't have finished with him
		Wire.endTransmission(false);
		//Ask the slave to read
		Wire.requestFrom(_address, buffLength);
		int readCount = Wire.readBytes(readBuffer, buffLength);
		return readCount;
	}
	return 0;
}
byte I2CBase::ReadFromRegister(byte reg)
{
	if (_initialized)
	{
		SetClockRate();
		//Advertise the slave device that we will need some data
		Wire.beginTransmission(_address);
		//Select the register to read from
		Wire.write(reg);
		//Send the instruction and Instruct the slave that we don't have finished with him
		Wire.endTransmission(false);
		//Ask the slave to read
		Wire.requestFrom((int)_address, 1);
		return Wire.read();
	}
	return 0;
}
//Set the clock rate in Khz before executing command
void I2CBase::SetClockRate()
{
	if (_initialized)
	{
		Wire.setClock(_clockRate*1000);
	}
}

//Instanciate I²C device with custom address and custom clock rate
I2CBase::I2CBase(byte address, int16_t clockRateKhz)
{
	_address = address;
	_clockRate = clockRateKhz;
}
//Instanciate I²C device with 400Khz clock rate
I2CBase::I2CBase(byte address) : I2CBase(address, 400)
{}

//Setup() is to be called inside the Setup() method
void I2CBase::Setup(uint8_t SDA, uint8_t SCL)
{
	//Initialize I²C interface
	_scl = SCL;
	_sda = SDA;
	Wire.begin(_sda, _scl);
	//Initialize clock rate for I²C
	Wire.setClock(_clockRate*1000);
	//Configure custom setup (this method is custom implemented)
	CustomSetup();
	//setup is done
	_initialized = true;
}

