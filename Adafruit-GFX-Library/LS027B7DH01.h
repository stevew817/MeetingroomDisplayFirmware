/***************************************************************************//**
 * @file LS013B7DH03.h
 * @brief Driver class for the Sharp LS013B7DH03 memory LCD on some kits.
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef SILABS_LS027B7DH01_H
#define SILABS_LS027B7DH01_H

#include "platform.h"
#include <mbed.h>
#include "LCDSettings.h"
#include "Adafruit_GFX.h"

#define LS027B7DH01_NO_ACTION		-3
#define	LS027B7DH01_OK				0

namespace silabs {
class LS027B7DH01 : public Adafruit_GFX {

public:

	LS027B7DH01(SPI * spi, DigitalOut * CS, DigitalOut * ExtCom, const char *name=NULL);

	/**
	 * Call this function to push all changes to the display
	 */
	int update( );

	/**
	 * Immediately clear the display: set pixel buffer to zero and clear out the display.
	 */
	int clearImmediate( );

	/**
	 * Function to get internal refresh counter
	 */
	uint32_t getRefreshTicks();
    
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void printf(const char* format, ...);

protected:
	mbed::SPI *_spi;
	mbed::DigitalOut *_EXTCOM;
	mbed::DigitalOut *_CS;

	mbed::LowPowerTicker _displayToggler;
	volatile uint32_t _refreshCount;
	uint8_t _lcdPolarity;
	uint8_t _cmd[5 + (DISPLAY_WIDTH / DISPLAY_BUFFER_TYPE_SIZE * sizeof(DISPLAY_BUFFER_TYPE))];
    
    DISPLAY_BUFFER_TYPE _pixelBuffer[DISPLAY_BUFFER_ELEMENTS]; // one full frame buffer
	DISPLAY_BUFFER_TYPE _dirtyRows[DISPLAY_HEIGHT/DISPLAY_BUFFER_TYPE_SIZE]; // 1 bit per row to indicate dirty status

	/**
	 * Call this function at 55 ~ 65 Hz to keep the display from losing contrast.
	 */
	void toggle();
};

} // namespace silabs

#endif //SILABS_LS027B7DH01_H
