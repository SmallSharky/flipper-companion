
#include "SH1106.hpp"
#include "PeriphWrappers.hpp"

#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SH1106_ACTIVATE_SCROLL 0x2F
#define SH1106_DEACTIVATE_SCROLL 0x2E
#define SH1106_SET_VERTICAL_SCROLL_AREA 0xA3
#define SH1106_RIGHT_HORIZONTAL_SCROLL 0x26
#define SH1106_LEFT_HORIZONTAL_SCROLL 0x27
#define SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

namespace app
{

    void SH1106::_cmd(uint8_t arg)
    {
        _dc.write(false);
        _spi.write(&arg, 1);
    }

    void SH1106::_data(const uint8_t *data, size_t len)
    {
        _dc.write(true);
        _spi.write(data, len);
    }

    void SH1106::init()
    {
        _rst.write(false);
        _cs.write(true);
        _dc.write(true);
        HAL_Delay(1);
        _rst.write(true);
        uint8_t vccstate = SH1106_EXTERNALVCC;
        _cs.write(false);
        _cmd(SH1106_DISPLAYOFF);                    // 0xAE
        _cmd(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
        _cmd(0x80);                                  // the suggested ratio 0x80
        _cmd(SH1106_SETMULTIPLEX);                  // 0xA8
        _cmd(0x3F);
        _cmd(SH1106_SETDISPLAYOFFSET);              // 0xD3
        _cmd(0x00);                                   // no offset
        _cmd(SH1106_SETSTARTLINE);
        
        _cmd(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
        _cmd(SH1106_CHARGEPUMP);                    // 0x8D
        if (vccstate == SH1106_EXTERNALVCC) 
        { _cmd(0x10); }
        else 
        { _cmd(0x14); }
        _cmd(SH1106_MEMORYMODE);                    // 0x20
        _cmd(0x00);                                  // 0x0 act like ks0108
        _cmd(SH1106_SEGREMAP | 0x1);
        _cmd(SH1106_COMSCANDEC);
        _cmd(SH1106_SETCOMPINS);                    // 0xDA
        _cmd(0x12);
        _cmd(SH1106_SETCONTRAST);                   // 0x81
        if (vccstate == SH1106_EXTERNALVCC) 
        { _cmd(0x9F); }
        else 
        { _cmd(0xCF); }
        _cmd(SH1106_SETPRECHARGE);                  // 0xd9
        if (vccstate == SH1106_EXTERNALVCC) 
        { _cmd(0x22); }
        else 
        { _cmd(0xF1); }
        _cmd(SH1106_SETVCOMDETECT);                 // 0xDB
        _cmd(0x40);
        _cmd(SH1106_DEACTIVATE_SCROLL);
        _cmd(SH1106_DISPLAYALLON_RESUME);           // 0xA4
        _cmd(SH1106_NORMALDISPLAY);                 // 0xA6
        _cmd(SH1106_DISPLAYON);
        _cs.write(true);
    }

    void SH1106::refresh(const std::array<uint8_t, w * h / 8> &buf)
    {
        _cs.write(false);
        for(size_t page = 0; page<(h>>3); ++page) {
            
            _cmd(0x02);
            _cmd(0x10);
            _cmd(0xB0 | page);

            _data(buf.data() + (page*w), w);
        }
        _cs.write(true);
    }
}