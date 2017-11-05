// 
// 
// 

#include "OLED_1306.h"

OLED_SSD1306::OLED_SSD1306(byte address, int16_t clockRateKhz) : I2CBase(address, clockRateKhz)
{
}

OLED_SSD1306::OLED_SSD1306(byte address) : I2CBase(address)
{
}
