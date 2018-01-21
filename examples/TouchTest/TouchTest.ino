/*
 Name:		TouchTest.ino
 Created:	21.01.2018 18:54:47
 Author:	Fox
*/

/* SPI-1 pinouts:
 * MOSI -> PA7
 * MISO -> PA6
 * SCLK -> PA5
 * NSS  -> PA4
 *
 * SPI-2 pinouts:
 * MOSI -> PB15
 * MISO -> PB14
 * SCLK -> PB13
 * NSS  -> PB12
 */

#include "SPI.h"
#include <XPT2046_STM.h>

// Pins defines
#define TOUCH_CS	PB0
#define TOUCH_IRQ	PB1		// Not realised in my library

SPIClass spi_First(1);
//SPIClass spi_Second(2);
XPT2046_STM ts(TOUCH_CS);

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	Serial.begin(115200);
	Serial.println(F("Starting..."));

	ts.begin(spi_First);
	Serial.println(F("Touch OK"));

	digitalWrite(LED_BUILTIN, LOW);
	Serial.println(F("Loop begin..."));
}

void loop(void) {
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

	TS_Point p = ts.getPoint();
	
	Serial.print(F("X: "));
	Serial.print(p.x);
	Serial.print(F("\tY: "));
	Serial.print(p.y);
	Serial.print(F("\tZ: "));
	Serial.println(p.z);

	delay(500);
}
