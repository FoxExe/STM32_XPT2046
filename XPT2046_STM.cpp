#include "XPT2046_STM.h"

TS_Point::TS_Point(void) {
	x = y = z = 0;
}

TS_Point::TS_Point(int16_t x0, int16_t y0, int16_t z0) {
	x = x0;
	y = y0;
	z = z0;
}

bool TS_Point::operator==(TS_Point p1) {
	return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool TS_Point::operator!=(TS_Point p1) {
	return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

/* ======================================================================= */

XPT2046_STM::XPT2046_STM(int8_t _cs_pin, int8_t _irq_pin)
{
	_pin_cs = _cs_pin;
	_pin_irq = _irq_pin;
}

void XPT2046_STM::begin(SPIClass & spi, uint32_t freq)
{
	pinMode(_pin_cs, OUTPUT);
	digitalWrite(_pin_cs, HIGH);
	pinMode(_pin_irq, INPUT);
	digitalWrite(_pin_irq, HIGH);
	_spi.begin();
}

TS_Point XPT2046_STM::getPoint()
{
	/*
	1 - Always 1 (Command enable)
	2, 3, 4 - A2, A1, A0 - Input select pin
	5 - Mode, 12(L) or 8(H) bit mode select
	6 - SER(H)/DFR(L) | Single-Ended (HIGH) / Differential reference Mode (LOW)
	7 - PD1, Power down mode
	8 - PD0 (LSB)
	A2,	A1,	A0
	0	0	0	= TEMP-0
	0	0	1	= Y
	0	1	0	= VBat
	0	1	1	= Z1
	1	0	0	= Z2
	1	0	1	= X
	1	1	0	= AUX-IN
	1	1	1	= TEMP-1
	
	# Request: B1XXX0000 00000000
	# Answer:  B0XXXXXXX XXXXX000

	R-touch = Rx-plate * X-Pos/4096 * (Z2 / Z1 - 1)
	R-touch = ((X-res * X-pos) / 4096) * (4096 / Z1 - 1) - Y-res * (1- (Y-Pos / 4096))
	*/

	digitalWrite(_pin_cs, LOW);

	_spi.write16(B11010000);	// Dummy offload
	_point.x = _spi.transfer16(0) >> 3;	// Raw X

	_spi.write16(B10010000);
	_point.y = _spi.transfer16(0) >> 3;	// Raw Y

	_spi.write16(B10110000);
	//_spi.write16(B11000000);	// Raw Z2
	_point.z = _spi.transfer16(0) >> 3;	// Raw Z1

	digitalWrite(_pin_cs, HIGH);
	return _point;
}

bool XPT2046_STM::touched()
{
	//TODO: Add irq handler
	// z ~= 3-5 by default. 100-1000 on press.
	getPoint();
	return (_point.z > 100)? true : false;
}
