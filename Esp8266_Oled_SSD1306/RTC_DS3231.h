// RTC_DS3231.h
#pragma once
#include "I2CBase.h"
#include "DateTime.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define RTC_DEFAULT_ADDRESS 0x68

enum class Alarm
{
	Alarm1 = 1,
	Alarm2 = 2
};

enum class AlarmMode
{
	OncePerSecond = 0x0F,
	WhenSecondsMatch = 0x0E,
	WhenMinutesSecondsMatch = 0x0C,
	WhenHoursMinutesSecondsMatch = 0x08,
	WhenDateHoursMinutesSecondsMatch = 0x00
};

class RTC_DS3231 : public I2CBase
{
public:
	RTC_DS3231(); //Don't forget to call Setup() before using
	RTC_DS3231(byte address);
	RTC_DS3231(byte address, uint16_t clockRateKhz);
	TimeSpan GetTime();
	Date GetDate();
	DateTime GetDateTime();
	void SetDate(uint16_t year, uint16_t month, uint16_t day);
	void SetTime24(uint16_t hours, uint16_t minute, uint16_t seconds);
	void SetTime12(bool isPM, uint16_t hours, uint16_t minutes, uint16_t seconds);
	void SetDay(DayOfWeek day);
	void SetAlarmDate(Alarm al, bool byDay, uint16_t day);
	void SetAlarmTime24(Alarm al, uint16_t hour, uint16_t minute, uint16_t second);
	void SetAlarmTime12(Alarm al, bool isPM, uint16_t hour, uint16_t minutes, uint16_t second);
	void EnableAlarm(Alarm al, AlarmMode mod);
	void ResetAlarm(Alarm al);
protected:
	void CustomSetup() override;

private:
	int16_t BCDToInt(byte value, TypeOfValue tov);
	uint16_t IntToBCD(byte value, TypeOfValue tov);
	const byte FuncSeconds = 0x00;
	const byte FuncMinutes = 0x01;
	const byte FuncHours = 0x02;
	const byte FuncDay = 0x03;
	const byte FuncDate = 0x04;
	const byte FuncMonth = 0x05;
	const byte FuncYear = 0x06;
	const byte FuncAlarm1Seconds = 0x07;
	const byte FuncAlarm1Minutes = 0x08;
	const byte FuncAlarm1Hours = 0x09;
	const byte FuncAlarm1Day = 0x0A;
	const byte FuncAlarm1Date = 0x0A;
	const byte FuncAlarm2Minutes = 0x0B;
	const byte FuncAlarm2Hours = 0x0C;
	const byte FuncAlarm2Day = 0x0D;
	const byte FuncAlarm2Date = 0x0D;
	const byte FuncControl = 0x0E;
	const byte FuncStatus = 0x0F;
	const byte FuncAgingOffset = 0x10;
	const byte FuncMSBTemp = 0x11;
	const byte FuncLSBTemp = 0x12;
};

