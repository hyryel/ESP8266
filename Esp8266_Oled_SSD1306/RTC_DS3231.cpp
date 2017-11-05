#include "RTC_DS3231.h"
//Create the RTC object with default address 0x68 and default clock rate (400 Khz)
RTC_DS3231::RTC_DS3231() : RTC_DS3231(RTC_DEFAULT_ADDRESS)
{
}
RTC_DS3231::RTC_DS3231(byte address) : RTC_DS3231(address, 400)
{
}
RTC_DS3231::RTC_DS3231(byte address, uint16_t clockRateKhz) : I2CBase(address, clockRateKhz)
{
}
//Get the time
TimeSpan RTC_DS3231::GetTime()
{
	TimeSpan result;
	byte* buffer = (byte*)calloc(1, sizeof(byte));
	ReadFromRegister(FuncSeconds, buffer, 1);
	result.Seconds = BCDToInt(buffer[0], TypeOfValue::SecOrMinOrMonthOrDate);

	buffer[0] = 0;
	ReadFromRegister(FuncMinutes, buffer, 1);
	result.Minutes = BCDToInt(buffer[0], TypeOfValue::SecOrMinOrMonthOrDate);

	buffer[0] = 0;
	ReadFromRegister(FuncHours, buffer, 1);
	result.Hours = BCDToInt(buffer[0], TypeOfValue::Hours24);

	free(buffer);
	return result;
}

//Get the date
Date RTC_DS3231::GetDate()
{
	Date date;
	byte* buffer = (byte*)calloc(1, sizeof(byte));
	//inform RTC we want to read date
	//read next byte and get them as date
	ReadFromRegister(FuncDate, buffer, 1);
	date.Day = BCDToInt(buffer[0], TypeOfValue::SecOrMinOrMonthOrDate);
	//inform RTC we want to read date
	//read next byte and get them as date
	ReadFromRegister(FuncMonth, buffer, 1);
	date.Month = BCDToInt(buffer[0], TypeOfValue::SecOrMinOrMonthOrDate);
	//inform RTC we want to read date
	//read next byte and get them as date
	ReadFromRegister(FuncYear, buffer, 1);
	date.Year = BCDToInt(buffer[0], TypeOfValue::Year);

	return date;
}

//Get the date and time
DateTime RTC_DS3231::GetDateTime()
{
	DateTime date;

	date.Time = GetTime();
	date.Day = GetDate();

	return date;
}

//Set the date
void RTC_DS3231::SetDate(uint16_t year, uint16_t month, uint16_t day)
{
	byte buf;
	buf = year % 99;
	buf = IntToBCD(buf, TypeOfValue::Year);
	WriteToRegister(FuncYear, buf);

	buf = month % 12;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncMonth, buf);

	buf = day % 31;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncDay, buf);

}
//Set the time in 24h format
void RTC_DS3231::SetTime24(uint16_t hours, uint16_t minutes, uint16_t seconds)
{
	byte buf;
	buf = hours % 24;
	buf = IntToBCD(buf, TypeOfValue::Hours24);
	WriteToRegister(FuncHours, buf);

	buf = minutes % 60;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncMinutes, buf);

	buf = seconds % 60;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncSeconds, buf);
}
//Set the time in 12h format
void RTC_DS3231::SetTime12(bool isPM, uint16_t hours, uint16_t minutes, uint16_t seconds)
{
	byte buf;
	buf = hours % 12;
	buf = IntToBCD(buf, TypeOfValue::Hours12);
	buf += 0x40; //set 12h format
	buf += isPM ? 0x20 : 0x00;
	WriteToRegister(FuncHours, buf);

	buf = minutes % 60;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncMinutes, buf);

	buf = seconds % 60;
	buf = IntToBCD(buf, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncSeconds, buf);
}
//Set the day of week.
void RTC_DS3231::SetDay(DayOfWeek day)
{
	WriteToRegister(FuncDay, (byte)day);
}
//Set the day/date of the alarm
//al : Alarm1 or Alarm2
//byDay : if true, day should be a value of DayOfWeek
//day : the day/date. Value is modulo 7 if byDay = true, else Modulo 31
void RTC_DS3231::SetAlarmDate(Alarm al, bool byDay, uint16_t day)
{
	byte adr;
	switch (al)
	{
	case Alarm::Alarm1:
		adr = FuncAlarm1Day;
		break;
	case Alarm::Alarm2:
		adr = FuncAlarm2Day;
		break;
	}
	byte value = byDay ? 0x40 : 0x00;
	value += IntToBCD(byDay ? day % 7 : day % 31, TypeOfValue::SecOrMinOrMonthOrDate);

	WriteToRegister(adr, value);

}
//Set the time for the alarm in 24h format
//al : Alarm1 or Alarm2
//hour : hour in 24h format. the value is modulo 24
//minute : the value is modulo 60
//second : the value is modulo 60
void RTC_DS3231::SetAlarmTime24(Alarm al, uint16_t hour, uint16_t minute, uint16_t second)
{
	byte adr;
	//set hour
	adr = al == Alarm::Alarm1 ? FuncAlarm1Hours : FuncAlarm2Hours;
	uint16_t val = IntToBCD(hour % 24, TypeOfValue::Hours24);
	WriteToRegister(adr, val);
	//set minutes
	adr = al == Alarm::Alarm1 ? FuncAlarm1Minutes : FuncAlarm2Minutes;
	val = IntToBCD(minute % 60, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(adr, val);
	if (al == Alarm::Alarm1)
	{
		//set second
		adr = FuncAlarm1Seconds;
		val = IntToBCD(second % 60, TypeOfValue::SecOrMinOrMonthOrDate);
		WriteToRegister(adr, val);
	}
}
//Set the time for the alarm in 12h format
//al : Alarm1 or Alarm2
//hour : hour in 12h format. the value is modulo 12
//minute : the value is modulo 60
//second : the value is modulo 60
void RTC_DS3231::SetAlarmTime12(Alarm al, bool isPM, uint16_t hour, uint16_t minute, uint16_t second)
{
	byte adr;
	//set hour
	adr = al == Alarm::Alarm1 ? FuncAlarm1Hours : FuncAlarm2Hours;
	uint16_t val = IntToBCD(hour % 12, TypeOfValue::Hours12);
	WriteToRegister(adr, val);
	//set minutes
	adr = al == Alarm::Alarm1 ? FuncAlarm1Minutes : FuncAlarm2Minutes;
	val = IntToBCD(minute % 60, TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(adr, val);
	if (al == Alarm::Alarm1)
	{
		//set second
		adr = FuncAlarm1Seconds;
		val = IntToBCD(second % 60, TypeOfValue::SecOrMinOrMonthOrDate);
		WriteToRegister(adr, val);
	}
}

void RTC_DS3231::EnableAlarm(Alarm al, AlarmMode mod)
{
	byte val;
	val = ReadFromRegister(FuncAlarm1Seconds);
	val = ((byte)mod & (byte)AlarmMode::OncePerSecond) == (byte)AlarmMode::OncePerSecond ? val | 0x80 : val & 0x7F ;
	WriteToRegister(FuncAlarm1Seconds, val); //deactivate seconds

	val = ReadFromRegister(al == Alarm::Alarm1 ? FuncAlarm1Minutes : FuncAlarm2Minutes);
	val = ((byte)mod & (byte)AlarmMode::WhenSecondsMatch) == (byte)AlarmMode::WhenSecondsMatch ? val | 0x80 : val & 0x7F;
	WriteToRegister(al == Alarm::Alarm1 ? FuncAlarm1Minutes : FuncAlarm2Minutes, val ); //activate minutes

	val = ReadFromRegister(al == Alarm::Alarm1 ? FuncAlarm1Hours : FuncAlarm2Hours);
	val = ((byte)mod & (byte)AlarmMode::WhenMinutesSecondsMatch) == (byte)AlarmMode::WhenMinutesSecondsMatch? val | 0x80 : val & 0x7F;
	WriteToRegister(al == Alarm::Alarm1 ? FuncAlarm1Hours : FuncAlarm2Hours, val | 0x80); //activate hours

	val = ReadFromRegister(al == Alarm::Alarm1 ? FuncAlarm1Date : FuncAlarm2Date);
	val = ((byte)mod & (byte)AlarmMode::WhenHoursMinutesSecondsMatch) == (byte)AlarmMode::WhenHoursMinutesSecondsMatch? val | 0x80 : val & 0x7F;
	WriteToRegister(al == Alarm::Alarm1 ? FuncAlarm1Date : FuncAlarm2Date, val ); //activate date
}
//Set alarm flag to off
void RTC_DS3231::ResetAlarm(Alarm al)
{
	byte val = ReadFromRegister(FuncStatus);
	WriteToRegister(FuncStatus, val & (al == Alarm::Alarm1 ? 0xFE : 0xFD ));
}


void RTC_DS3231::CustomSetup()
{
	return I2CBase::CustomSetup();
}

int16_t RTC_DS3231::BCDToInt(byte value, TypeOfValue tov)
{
	short val = 0;
	switch (tov)
	{
#pragma region SecMinMonthDate
	case TypeOfValue::SecOrMinOrMonthOrDate:
		// value mapping :    7654 3210
		//                     0000 0000
		// bit 7 : unused
		// bit 6 : \
		// bit 5 :  => seconds * 10 
		// bit 4 : /
		// bit 3 : \
		// bit 2 :  \
		// bit 1 :   => seconds * 1
		// bit 0 :  /
		val = ((value & 0x70) >> 4) * 10;
		val += (value & 0x0F);
		break;
#pragma endregion
#pragma region Hours
	case TypeOfValue::Hours24:
	case TypeOfValue::Hours12:
		// buffer mapping :    7654 3210
		//                     0000 0000
		// bit 7 : unused
		// bit 6 : 1 = 12h, 0 = 24h
		// bit 5 : bit6 = 1 => 0 = AM, 1 = PM | bit6 = 0 => 1 = 20 hours
		// bit 4 : hours * 10
		// bit 3 : \
		// bit 2 :  \
		// bit 1 :   => hours * 1
		// bit 0 :  /
		if ((value & 0x40) == 1) // 12h mode
		{
			val = (value & 0x20) == 0x20 ? 12 : 0;
		}
		else // 24h mode
		{
			val = (value & 0x20) == 0x20 ? 20 : 0;
		}
		val += (value & 0x10) == 0x10 ? 10 : 0;
		val += (value & 0x0F);
		break;
#pragma endregion
#pragma region Year
	case TypeOfValue::Year:
		// buffer mapping :    7654 3210
		//                     0000 0000
		// bit 7 : \
		// bit 6 :  \
		// bit 5 :   => year * 10 
		// bit 4 : _/
		// bit 3 : \
		// bit 2 :  \
		// bit 1 :   => year * 1
		// bit 0 :  /
		val = ((value & 0xF0) >> 4) * 10;
		val += (value & 0x0F);
		break;
#pragma endregion
	}
	return val;

}

uint16_t RTC_DS3231::IntToBCD(byte value, TypeOfValue tov)
{
	uint16_t val = 0;
	switch (tov)
	{
	case TypeOfValue::SecOrMinOrMonthOrDate:
		val = (value / 10) << 4;
		val += value % 10;
		val &= 0x7F;
		break;
	case TypeOfValue::Hours24:
		val = (value / 20 >= 1) ? 0x20 : (value / 10 >= 1 ? 0x10 : 0x00);
		val += (value % 10);
		break;
	case TypeOfValue::Hours12:
	case TypeOfValue::Year:
		val = (value / 10) << 4;
		val += value % 10;
		break;
	}
	return val;
}


