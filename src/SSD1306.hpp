#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace app
{
    struct GPIO;
    struct SPI;

    class SSD1306
    {
    public:
        static constexpr size_t w = 128;
        static constexpr size_t h = 64;
        SSD1306(SPI &spi, GPIO &cs, GPIO &dc, GPIO &rst) : _spi(spi), _cs(cs), _dc(dc), _rst(rst)
        {
        }

        void init();
        void blink();
        void refresh(const std::array<uint8_t, w * h / 8> &buf);

    private:
        SPI &_spi;
        GPIO &_cs;
        GPIO &_dc;
        GPIO &_rst;

        void _cmd(uint8_t arg);

        void _data(uint8_t *data, size_t len);
    };
}