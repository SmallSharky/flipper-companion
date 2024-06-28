
#include "SSD1306.hpp"
#include "PeriphWrappers.hpp"

namespace app
{

    void SSD1306::_cmd(uint8_t arg)
    {
        _dc.write(false);
        _spi.write(&arg, 1);
    }

    void SSD1306::_data(uint8_t *data, size_t len)
    {
        _dc.write(true);
        _spi.write(data, len);
    }

    void SSD1306::init()
    {
        _cs.write(false);
        _cmd(0xae); /* display off */
        _cmd(0xd5);
        _cmd(0x80); /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
        _cmd(0xa8);
        _cmd(0x3f); /* multiplex ratio */
        _cmd(0xd3);
        _cmd(0x00); /* display offset */
        _cmd(0x40); /* set display start line to 0 */
        _cmd(0x8d);
        _cmd(0x14); /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable, SSD1306 only, should be removed for SH1106 */
        _cmd(0x20);
        _cmd(0x00); /* horizontal addressing mode */

        _cmd(0xa1); /* segment remap a0/a1*/
        _cmd(0xc8); /* c0: scan dir normal, c8: reverse */
        // Flipmode
        // _cmd(0xa0),				/* segment remap a0/a1*/
        // _cmd(0xc0),				/* c0: scan dir normal, c8: reverse */

        _cmd(0xda);
        _cmd(0x12); /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */

        _cmd(0x81);
        _cmd(0xcf); /* [2] set contrast control */
        _cmd(0xd9);
        _cmd(0xf1); /* [2] pre-charge period 0x022/f1*/
        _cmd(0xdb);
        _cmd(0x40); /* vcomh deselect level */
        // if vcomh is 0, then this will give the biggest range for contrast control issue #98
        // restored the old values for the noname constructor, because vcomh=0 will not work for all OLEDs, #116

        _cmd(0x2e); /* Deactivate scroll */
        _cmd(0xa4); /* output ram to display */
        _cmd(0xa6); /* none inverted normal display mode */
        _cs.write(true);
    }

    void SSD1306::refresh(const std::array<uint8_t, w * h / 8> &buf)
    {
        _cs.write(false);

        _cmd(0x22); // Set page address
        _cmd(0);    // Page start address
        _cmd(0xFF); // Page end (not really, but works here)
        _cmd(0x21);
        _cmd(0); // Column start address

        _cmd(w - 1); // Column end address

        uint8_t tmp[w>>3];
        for(size_t y = 0; y<h; ++y) {
            size_t bufOffset = y>>3;
            size_t byteOffset = y&0x07;
            for(size_t x = 0; x<w; ++x) {                
                uint8_t pxv = (buf[bufOffset+x]&(1<<byteOffset));
                if(pxv) {
                    tmp[x>>3] |= 1<<(x&0x07);
                } else {
                    tmp[x>>3] &= ~(1<<(x&0x07));
                }                
            }
            _data(tmp, sizeof(tmp));
        }
        _cs.write(true);
    }

    void SSD1306::blink() {
        _cs.toggle();
        _dc.toggle();
        _rst.toggle();
        uint8_t data = 0b10101010;
        _spi.write(&data, 1);
    }
}