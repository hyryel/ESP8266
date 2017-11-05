// OLED_1306.h


#ifndef _OLED_1306_h
#define _OLED_1306_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "I2CBase.h"

class OLED_SSD1306 : I2CBase
{
public:
	OLED_SSD1306(byte address, int16_t clockRateKhz);
	OLED_SSD1306(byte address);

protected :

private:

};

#endif

