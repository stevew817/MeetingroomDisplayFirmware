/***************************************************************************//**
 * @file LS013B7DH03.cpp
 * @brief Driver class for the Sharp LS013B7DH03 memory LCD on some kits.
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, http://www.silabs.com</b>
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

#include <mbed.h>
#include "LS027B7DH01.h"
#include "SPI.h"

/* LS013B7DH03 SPI commands */
#define LS027B7DH01_CMD_UPDATE     (0x01)
#define LS027B7DH01_CMD_ALL_CLEAR  (0x04)

/* Macro to switch endianness on char value */
#define SWAP8(a) ((((a) & 0x80) >> 7) | (((a) & 0x40) >> 5) | (((a) & 0x20) >> 3) | (((a) & 0x10) >> 1) | (((a) & 0x08) << 1) | (((a) & 0x04) << 3) | (((a) & 0x02) << 5) | (((a) & 0x01) << 7))

namespace silabs {

LS027B7DH01::LS027B7DH01(mbed::SPI * spi, DigitalOut * CS, DigitalOut * ExtCom, const char *name) : Adafruit_GFX(DISPLAY_WIDTH, DISPLAY_HEIGHT) {
	//Save pointer to ChipSelect pin
	_CS = CS;
	_CS->write(0);

	//Save pointer to ExtCom pin
	_EXTCOM = ExtCom;
	_EXTCOM->write(0);

	//Save pointer to spi peripheral
	_spi = spi;

	//Initialize
	_spi->set_dma_usage((DMAUsage)DMA_USAGE_NEVER);
	_refreshCount = 0;
	_lcdPolarity = 0;

    memset(_pixelBuffer, 0, sizeof(_pixelBuffer));
    memset(_dirtyRows, 0, sizeof(_dirtyRows));

	//Start toggling the EXTCOM pin
	_displayToggler.attach(callback(this, &LS027B7DH01::toggle), 0.5f);
}

/**
 * Call this function at 55 ~ 65 Hz to keep the display up-to-date.
 */
void LS027B7DH01::toggle() {
	_EXTCOM->write(!_EXTCOM->read());
	_refreshCount++;
}

/**
 * Function to get internal refresh counter
 */
uint32_t LS027B7DH01::getRefreshTicks() {
	return _refreshCount;
}

/**
 * Call this function to push all changes to the display
 */
int LS027B7DH01::update( ) {
	uint32_t rowCount = 0;
	bool update = false;

	// Check if something actually changed in the pixelbuffer
	for(rowCount = 0; rowCount < DISPLAY_HEIGHT/DISPLAY_BUFFER_TYPE_SIZE; rowCount++) {
		if(_dirtyRows[rowCount] != 0) update = true;
	}

	if(update == false) return LS027B7DH01_NO_ACTION;

	// Take control
	rowCount = 0;

	//Initialize the command vector
	_cmd[0] = (uint8_t)SWAP8(LS027B7DH01_CMD_UPDATE);
	_cmd[1] = SWAP8(1);

    // acquire SPI bus
    _spi->lock();

	// Activate LCD
	_CS->write(1);
    Thread::wait(10);
    
    while(rowCount < DISPLAY_HEIGHT) {
        // Determine the next line to send
        if((_dirtyRows[rowCount / DISPLAY_BUFFER_TYPE_SIZE] & (1 << (rowCount % DISPLAY_BUFFER_TYPE_SIZE))) != 0) {

            // Row is dirty, send an update to the display
            _cmd[1] = (uint8_t)SWAP8(rowCount + 1);
            memcpy((void*)&(_cmd[2]), (const void*)&(_pixelBuffer[rowCount*(DISPLAY_WIDTH/DISPLAY_BUFFER_TYPE_SIZE)]), DISPLAY_WIDTH / DISPLAY_BUFFER_TYPE_SIZE * sizeof(DISPLAY_BUFFER_TYPE));
            
            _spi->write((const char*)_cmd, 2 + DISPLAY_WIDTH / DISPLAY_BUFFER_TYPE_SIZE * sizeof(DISPLAY_BUFFER_TYPE), NULL, 0);

            // Transaction is done
            _dirtyRows[rowCount / DISPLAY_BUFFER_TYPE_SIZE] &= ~(1 << (rowCount % DISPLAY_BUFFER_TYPE_SIZE));
        }
        // Row wasn't touched, so check the next row
        rowCount++;
    }
    
    _spi->write(0x00);
	
    Thread::wait(10);
    _CS->write(0);

    // release SPI bus
    _spi->unlock();
    Thread::wait(10);
	return LS027B7DH01_OK;
}

/**
 * Call this function to immediately clear the display
 */
int LS027B7DH01::clearImmediate( ) {
	// Clear out the pixel buffer
	memset((void*)_pixelBuffer, White, sizeof(_pixelBuffer));
	memset((void*)_dirtyRows, 0, sizeof(_dirtyRows));

	_cmd[0] = (uint8_t)(SWAP8(LS027B7DH01_CMD_ALL_CLEAR | _lcdPolarity));
	_cmd[1] = 0;

    // acquire SPI bus
    _spi->lock();
	// Wait for the ChipSelect line
	_CS->write(1);
	Thread::wait(20);

    _spi->write((const char*)_cmd, 2, NULL, 0);

    Thread::wait(200);
    _CS->write(0);
    // release SPI bus
    _spi->unlock();

    Thread::wait(10);
	return LS027B7DH01_OK;
}

void LS027B7DH01::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    /* Apply constraint to x and y */
    if(x < 0 || y < 0) return;
    if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
    
    /***************************************************************************************************************** 
     * The display expects LSB input, while the SPI is configured for 8bit MSB transfers. Therefore, we should  
     * construct the framebuffer accordingly, so that an MSB transmission will put pixel 0 first on the wire.
     *
     * So the actual pixel layout in framebuffer (for 128x128) is as follows:
     * {                                                    //Framebuffer
     *	{                                                   //Line 0
     *	 {p0, p1, p2, p3, p4, p5, p6, p7},                  //Line 0 byte 0 (byte 0)
     *   {p8, p9,p10,p11,p12,p13,p14,p15},                  //Line 0 byte 1 (byte 1)
     *   ...
     *   {p120,p121,p122,p123,p124,p125,p126,p127}          //Line 0 byte 15 (byte 15)
     *  },        
     *  {													//Line 1
     *	 {p128,p129,p130,p131,p132,p133,p134,p135},         //Line 1 byte 0 (byte 16)
     *   ...
     *  },
     *  ...
     *  {													//Line 127
     *	 {...},              								//Line 127 byte 0 (byte 2032)
     *   ...
     *   {...}												//Line 127 byte 15 (byte 2047) = 128*128 bits
     *	}
     * }
     *
     * This means that to calculate the actual bit position in the framebuffer byte, we need to swap the bit 
     * order of the lower three bits. So pixel 7 becomes bit offset 0, 6 -> 1, 5 -> 2, 4->3, 3->4, 2->5, 1->6 and 0->7.
     *****************************************************************************************************************/
    uint8_t swapx = 7 - ((unsigned int)x & 0x07);
    x = ((unsigned int)x & 0xFFFFFFF8) | swapx;

    /* Since we are dealing with 1-bit pixels, we can avoid having to do bitshift and comparison operations twice.
     * Basically, do the comparison with the requested state and current state, and if it changed, do an XOR on the framebuffer pixel and set the line to dirty.
     */
    bool change = ((_pixelBuffer[((y * DISPLAY_WIDTH) + x) / DISPLAY_BUFFER_TYPE_SIZE] & (1 << (x & DISPLAY_BUFFER_TYPE_MASK))) != ((color & 0x01) << (x & DISPLAY_BUFFER_TYPE_MASK)));
    if(change) {
        /* Pixel's value and requested value are different, so since it's binary, we can simply do an XOR */
        _pixelBuffer[((y * DISPLAY_WIDTH) + x) / DISPLAY_BUFFER_TYPE_SIZE] ^= (1 << (x & DISPLAY_BUFFER_TYPE_MASK));

        /* notify dirty status of this line */
        _dirtyRows[y / DISPLAY_BUFFER_TYPE_SIZE] |= (1 << (y & DISPLAY_BUFFER_TYPE_MASK));
    }
}

void LS027B7DH01::printf(const char* format, ...) {
	char buffer[MAX_PRINTF_CHARS + 1] = { 0 };
	uint32_t iterator = 0;
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	while((buffer[iterator] != 0) && (iterator < MAX_PRINTF_CHARS)) {
		this->write(buffer[iterator++]);
	}
}

} // namespace silabs
