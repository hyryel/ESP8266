#pragma once
class TimeSpan
{
public:
	uint16 Seconds;
	uint16_t Minutes;
	uint16_t Hours;
	char* ToString()
	{
		char* buffer = (char*)calloc(9, sizeof(char));
		snprintf(buffer, 9, "%02d:%02d:%02d", Hours, Minutes, Seconds);

		return buffer;
	};
};

class Date
{
public:
	uint16_t Day;
	uint16_t Month;
	uint16_t Year;
	char* ToString()
	{
		char* buffer = (char*)calloc(11, sizeof(char));

		snprintf(buffer, 11, "%02d/%02d/%02d", Year, Month, Day);
		return buffer;
	};
};

class DateTime
{
public:
	TimeSpan Time;
	Date Day;
	char* ToString()
	{
		char* s = (char*)calloc(18, sizeof(char));

		snprintf(s, 20, "%02d/%02d/%02d %02d:%02d:%02d", Day.Year, Day.Month, Day.Day, Time.Hours, Time.Minutes, Time.Seconds);

		return s;
	};
};

enum class TypeOfValue
{
	SecOrMinOrMonthOrDate,
	Hours24,
	Hours12,
	Year
};
enum class DayOfWeek
{
	Monday = 1,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
};


