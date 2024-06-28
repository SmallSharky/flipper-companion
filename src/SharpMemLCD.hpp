#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

#include "DisplayInterface.hpp"




namespace app{
class SharpMemLCD
{
public:
    SharpMemLCD(DisplayInterface * iface);
    //   Adafruit_SharpMem(uint8_t clk, uint8_t mosi, uint8_t cs, uint16_t w = 96,
    //                     uint16_t h = 96, uint32_t freq = 2000000);
    //   Adafruit_SharpMem(SPIClass *theSPI, uint8_t cs, uint16_t w = 96,
    //                     uint16_t h = 96, uint32_t freq = 2000000);
    //   boolean begin();
    void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    uint8_t getPixel(uint16_t x, uint16_t y);
    
    void refresh();
    void clearWithSync();
    void clearBuffer();

    uint16_t w() {
        return _w;
    }

    uint16_t h() {
        return _h;
    }

private:
    void _init();

    uint8_t _vcom();
    uint8_t _vcomState{0};

    DisplayInterface * _iface;
    static const uint16_t _w{336};
    static const uint16_t _h{536};
    std::array<uint8_t, ((_w * _h) / 8)> _buf;
};
}