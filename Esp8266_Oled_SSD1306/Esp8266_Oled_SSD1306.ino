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
		elapsed = millis();
	}
}

