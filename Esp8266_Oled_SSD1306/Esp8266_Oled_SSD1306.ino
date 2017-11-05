/*
 Name:		Esp8266_Oled_SSD1306.ino
 Created:	11/4/2017 4:14:52 PM
 Author:	Nicolas
*/

// the setup function runs once when you press reset or power the board

#include "RTC_DS3231.h"
RTC_DS3231 clock;

void setup()
{
	Serial.begin(115200);
	clock.Setup(SDA, SCL);
}
ulong elapsed = 0;
ulong Interval = 1000;
// the loop function runs over and over again until power down or reset
void loop()
{

	if (millis() - elapsed > Interval)
	{
		TimeSpan time = clock.GetTime();
		Date day = clock.GetDate();
		DateTime dateTime = clock.GetDateTime();
		char* sdateTime = dateTime.ToString();
		char* stime = time.ToString();
		char* sday = day.ToString();

		Serial.printf("%s %s\t%s\n", sday, stime, sdateTime);

		free(stime);
		free(sday);
		free(sdateTime);

		ClockStatus status = clock.ReadStatus();
		Serial.print("Alarm 1 is " );
		Serial.println(status.Alarm1FlagOn ? "On" : "Off");
		Serial.print("Alarm 2 is " );
		Serial.println(status.Alarm2FlagOn ? "On" : "Off");
		Serial.print("32 Khz output is ");
		Serial.println(status.Output32KhEnabled ? "On" : "Off");
		Serial.print("Clock is ");
		Serial.println(status.IsBusy ? "Busy" : "Available");
		Serial.print("Oscillator has stopped  : ");
		Serial.println(status.OscillatorHasStopped ? "Yes" : "No");

		elapsed = millis();
	}
}

