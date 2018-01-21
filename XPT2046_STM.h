#pragma once
#include "Arduino.h"
#include <SPI.h>

class TS_Point {
public:
	TS_Point(void);
	TS_Point(int16_t x, int16_t y, int16_t z);

	bool operator==(TS_Point);
	bool operator!=(TS_Point);

	int16_t x, y, z;
};

class XPT2046_STM
{
public:
	XPT2046_STM(int8_t _cs_pin, int8_t _irq_pin = -1);
	void begin(SPIClass & spi, uint32_t freq = 24000000ul);
	void begin(void) { begin(SPI); }

	TS_Point getPoint();
	boolean touched();
private:
	int8_t _pin_cs, _pin_irq;
	SPIClass & _spi = SPI;
	TS_Point _point = { 0, 0, 0 };
};

