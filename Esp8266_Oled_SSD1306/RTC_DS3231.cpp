// 
// 
// 

#include "RTC_DS3231.h"

RTC_DS3231::RTC_DS3231() : I2CBase(RTC_DEFAULT_ADDRESS)
{
}

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

DateTime RTC_DS3231::GetDateTime()
{
	DateTime date;

	date.Time = GetTime();
	date.Day = GetDate();

	return date;
}

void RTC_DS3231::SetDate(uint16_t year, uint16_t month, uint16_t day)
{
	byte buf[1];
	buf[0] = year > 99 ? 99: year;
	buf[0] = IntToBCD(buf[0], TypeOfValue::Year);
	WriteToRegister(FuncYear, buf, 1);

	buf[0] = month > 12 ? month % 12 : month;
	buf[0] = IntToBCD(buf[0], TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncMonth, buf, 1);

	buf[0] = day > 31 ? day % 31 : day;
	buf[0] = IntToBCD(buf[0], TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncDay, buf, 1);

}

void RTC_DS3231::SetTime(uint16_t hours, uint16_t minutes, uint16_t seconds)
{
	byte buf[1];
	buf[0] = hours > 24 ? hours % 24 : hours;
	buf[0] = IntToBCD(buf[0], TypeOfValue::Hours24);
	WriteToRegister(FuncHours, buf, 1);

	buf[0] = minutes > 59 ? minutes % 60 : minutes;
	buf[0] = IntToBCD(buf[0], TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncMinutes, buf, 1);

	buf[0] = seconds > 59 ? seconds % 60 : seconds;
	buf[0] = IntToBCD(buf[0], TypeOfValue::SecOrMinOrMonthOrDate);
	WriteToRegister(FuncSeconds, buf, 1);

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
		val += (value / 20 >= 1) ? 0x20 :( value / 10 >= 1 ? 0x10 : 0x00);
		val += (value % 10);
		break;
	case TypeOfValue::Hours12:
		break;
	case TypeOfValue::Year:
		val = (value / 10) << 4;
		val += value % 10;
		break;
	}
	return val;
}

char* TimeSpan::ToString()
{
	char* buffer = (char*)calloc(9, sizeof(char));
	snprintf(buffer, 9, "%02d:%02d:%02d", Hours, Minutes, Seconds);

	return buffer;
}

char * Date::ToString()
{
	char* buffer = (char*)calloc(11, sizeof(char));

	snprintf(buffer, 11, "%02d/%02d/%02d", Year, Month, Day);
	return buffer;
}

char * DateTime::ToString()
{
	char* s = (char*)calloc(18, sizeof(char));

	snprintf(s, 20, "%02d/%02d/%02d %02d:%02d:%02d", Day.Year, Day.Month, Day.Day, Time.Hours, Time.Minutes, Time.Seconds);

	return s;
}
