/*********************************************************************
This is an Arduino library for our Monochrome SHARP Memory Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1393

These displays use SPI to communicate, 3 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include "SharpMemLCD.hpp"
#include "main.h"
#include <cstring>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif
#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) \
    {                        \
        uint16_t t = a;      \
        a = b;               \
        b = t;               \
    }
#endif

static const uint8_t SHARPMEM_BIT_WRITECMD = 0x01; // 0x80 in LSB format
static const uint8_t SHARPMEM_BIT_VCOM = 0x02;     // 0x40 in LSB format
static const uint8_t SHARPMEM_BIT_CLEAR = 0x04;    // 0x20 in LSB format

// static const uint8_t SHARPMEM_BIT_WRITECMD = 0x80; // 0x80 in LSB format
// static const uint8_t SHARPMEM_BIT_VCOM = 0x40;     // 0x40 in LSB format
// static const uint8_t SHARPMEM_BIT_CLEAR = 0x20;    // 0x20 in LSB format


/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Pin   Function        Notes
    ===   ==============  ===============================
      1   VIN             3.3-5.0V (into LDO supply)
      2   3V3             3.3V out
      3   GND
      4   SCLK            Serial Clock
      5   MOSI            Serial Data Input
      6   CS              Serial Chip Select
      9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
      7   EXTCOMIN        External COM Inversion Signal
      8   DISP            Display On(High)/Off(Low)

 **************************************************************************/

#define TOGGLE_VCOM                                                 \
    do                                                              \
    {                                                               \
        _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; \
    } while (0);



namespace app{
/**
 * @brief Construct a new Adafruit_SharpMem object with software SPI
 *
 * @param clk The clock pin
 * @param mosi The MOSI pin
 * @param cs The display chip select pin - **NOTE** this is ACTIVE HIGH!
 * @param width The display width
 * @param height The display height
 * @param freq The SPI clock frequency desired (unlikely to be that fast in soft
 * spi mode!)
 */
SharpMemLCD::SharpMemLCD(DisplayInterface * iface): _iface(iface) {
    _init();
}

void SharpMemLCD::_init() {
    memset(_buf.data(), 0xff, (_w * _h) / 8);
    refresh();
    // this display is weird in that _cs is active HIGH not LOW like every other
    // SPI device
    // HAL_GPIO_WritePin(DISPLAY_CS_PORT, DISPLAY_CS_PIN, GPIO_PIN_RESET);
}



// // 1<<n is a costly operation on AVR -- table usu. smaller & faster
// static const uint8_t PROGMEM set[] = {1, 2, 4, 8, 16, 32, 64, 128},
//                              clr[] = {(uint8_t)~1, (uint8_t)~2, (uint8_t)~4,
//                                       (uint8_t)~8, (uint8_t)~16, (uint8_t)~32,
//                                       (uint8_t)~64, (uint8_t)~128};

/**************************************************************************/
/*!
    @brief Draws a single pixel in image buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)
    @param color The color to set:
    * **0**: Black
    * **1**: White
*/
/**************************************************************************/
void SharpMemLCD::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= _w) || (y >= _h))
        return;
    

    if (color)
    {
        _buf[(y*_w + x)/8] |= 1<<(x&7);
        // sharpmem_buffer[(y * _w + x) / 8] |= pgm_read_byte(&set[x & 7]);
    }
    else
    {
        _buf[(y*_w + x)/8] &= ~(1<<(x&7));
    }
}

/**************************************************************************/
/*!
    @brief Gets the value (1 or 0) of the specified pixel from the buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)

    @return     1 if the pixel is enabled, 0 if disabled
*/
/**************************************************************************/
uint8_t SharpMemLCD::getPixel(uint16_t x, uint16_t y)
{
    if ((x >= _w) || (y >= _h)){
        return 0; // <0 test not needed, unsigned
    }
        

    return _buf[((y*_w)+x) / 8] & (1<<(x & 7)) ? 1 : 0;
}

/**************************************************************************/
/*!
    @brief Clears the screen
*/
/**************************************************************************/
void SharpMemLCD::clearWithSync()
{
    memset(_buf.data(), 0x00, (_w * _h) / 8);
    const uint8_t cleanSequence[] = {(uint8_t)(uint8_t(_vcom()) | uint8_t(SHARPMEM_BIT_CLEAR)), uint8_t(0x00)};
    _iface->writeTransaction(cleanSequence, sizeof(cleanSequence));

    // spidev->beginTransaction();
    // // Send the clear screen command rather than doing a HW refresh (quicker)
    // digitalWrite(_cs, HIGH);

    // uint8_t clear_data[2] = {(uint8_t)(_sharpmem_vcom | SHARPMEM_BIT_CLEAR),
    //                          0x00};
    // spidev->transfer(clear_data, 2);

    // TOGGLE_VCOM;
    // digitalWrite(_cs, LOW);
    // spidev->endTransaction();
}

/**************************************************************************/
/*!
    @brief Renders the contents of the pixel buffer on the LCD
*/
/**************************************************************************/
void SharpMemLCD::refresh(void)
{
    uint16_t i, currentline;
    
    // spidev->beginTransaction();
    // Send the write command
    // digitalWrite(_cs, HIGH);

    // spidev->transfer(_sharpmem_vcom | SHARPMEM_BIT_WRITECMD);
    // uint8_t payload[] = {SHARPMEM_BIT_WRITECMD};
    {
        DisplayInterface::Transaction t{*_iface};
        _iface->write(SHARPMEM_BIT_WRITECMD | _vcom());
        // TOGGLE_VCOM;

        const uint8_t bytes_per_line = _w / 8;
        uint16_t totalbytes = (_w * _h) / 8;

        for (i = 0; i < totalbytes; i += bytes_per_line)
        {
            uint8_t line[bytes_per_line + 2];

            // Send address byte
            currentline = ((i + 1) / (_w / 8)) + 1;
            line[0] = currentline;
            // copy over this line
            memcpy(line + 1, _buf.data() + i, bytes_per_line);
            // Send end of line
            line[bytes_per_line + 1] = 0x00;
            // send it!
            _iface->write(line, bytes_per_line + 2);
        }

        // Send another trailing 8 bits for the last line
        _iface->write(0x00);
    }
    // {
    //     DisplayInterface::Transaction t{*_iface};
    //     _iface->write(0x00);
    //     _iface->write(0x00);
    // }

    // digitalWrite(_cs, LOW);
    // spidev->endTransaction();
}

/**************************************************************************/
/*!
    @brief Clears the display buffer without outputting to the display
*/
/**************************************************************************/
void SharpMemLCD::clearBuffer()
{
    memset(_buf.data(), 0xff, (_w * _h) / 8);
}


uint8_t SharpMemLCD::_vcom() {
    _vcomState = _vcomState?0x00:SHARPMEM_BIT_VCOM;
    return _vcomState;
}

} // namespace app