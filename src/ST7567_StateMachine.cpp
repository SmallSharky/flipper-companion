#include "ST7567_StateMachine.hpp"

#include <cstdio>

// clang-format off
#define ST7567A_DISPLAY_ON                  0xAF
#define ST7567A_DISPLAY_OFF                 0xAE
#define ST7567A_SET_START_LINE              0x40   // + line0 - line63 (5 bits)
#define ST7567A_SET_START_LINE_MASK         0x1F
#define ST7567A_SEG_NORMAL                  0xA0
#define ST7567A_SEG_REVERSE                 0xA1
#define ST7567A_COLOR_NORMAL                0xA6
#define ST7567A_COLOR_INVERSE               0xA7
#define ST7567A_DISPLAY_DRAM                0xA4
#define ST7567A_DISPLAY_ALL_ON              0xA5
#define ST7567A_SW_RESET                    0xE2
#define ST7567A_COM_NORMAL                  0xC0
#define ST7567A_COM_REVERSE                 0xC8
#define ST7567A_POWER_CONTROL               0x28   // + X[2:0]
#define ST7567A_POWER_CONTROL_MASK          0x07
#define ST7567A_SET_RR                      0x20   // Set Regulation Ratio + RR[2:0]; 3.0, 3.5, ..., 6.5
#define ST7567A_SET_RR_MASK                 0x07
#define ST7567A_SET_EV_CMD                  0x81
#define ST7567A_NOP                         0xE3
#define ST7567A_BIAS7                       0xA3
#define ST7567A_BIAS9                       0xA2
#define ST7567A_PWR_BOOSTER_ON              0x04
#define ST7567A_PWR_REGULATOR_ON            0x02
#define ST7567A_PWR_FOLLOWER_ON             0x01
#define ST7567A_SET_BOOSTER                 0xF8
#define ST7567A_REGULATION_RATIO            0x20
#define ST7567A_REGULATION_RATIO_MASK       0x07

#define ST7567A_PAGE_ADDR                   0xB0   // + 0x0 - 0x7 -> page0 - page7
#define ST7567A_PAGE_ADDR_MASK              0x0F
#define ST7567A_COL_ADDR_H                  0x10   // + X[7:4]
#define ST7567A_COL_ADDR_L                  0x00   // + X[3:0]
#define ST7567A_COL_ADDR_MASK               0x0F
// clang-format on

namespace app
{
    void ST7567_StateMachine::process(uint8_t value, bool isCommand)
    {

        if (isCommand)
        {
            _processData(value);
        }
        else
        {
            _processCMD(value);
        }
        
        if (_redrawState & 1 << 4) {
            _updated = true;
        }
    }

    void ST7567_StateMachine::_setPixel(uint8_t x, uint8_t y, bool value)
    {
        size_t index = (y * W + x) >> 3; // >>3 means dividing by 8
        uint8_t mask = 1 << (x & 0x07);  // x&0x07 means x%8
        buffer[index] = (buffer[index] & ~mask) | (value ? mask : 0);
    }

    void ST7567_StateMachine::display()
    {
        for (size_t y = 0; y < (H >> 3); ++y)
        {
            size_t pageOffset = y * W;
            for (size_t i = 0; i < 8; ++i)
            {
                for (size_t x = 0; x < W; ++x)
                {
                    char c = (buffer[pageOffset + x] & (1 << i) ? 'X' : ' ');
                    putchar(c);
                    putchar(c);
                }
                putchar('\n');
            }
        }
        putchar('\n');
        putchar('\n');
        putchar('\n');
        putchar('\n');
        fflush(stdout);
    }

    void ST7567_StateMachine::_processData([[maybe_unused]] uint8_t value)
    {
        if ((_redrawState & 0b1) != 0b1)
        {
            if ((value | ST7567A_PAGE_ADDR_MASK) == (ST7567A_PAGE_ADDR | ST7567A_PAGE_ADDR_MASK))
            {
                if (_column > 2)
                {
                    return;
                }
                _redrawState |= 1 << 0;
                _page = value & ST7567A_PAGE_ADDR_MASK;
                _column = 0;
            }
            _column++;
            return;
        }
        _redrawState |= 1 << 4;
        if (_column < W)
        {
            size_t offset = _page * W;
            buffer[offset + _column] = value;
        }
        _column++;
    }

    void ST7567_StateMachine::_processCMD(uint8_t value)
    {
        if (_redrawState & 1 << 4)
        {
            
            _redrawState = 0;
            _column = 0;
            _page = 0;
        }
        if (_cmdState != CmdState::NORMAL)
        {
            // For multiline commands
            _cmdState = CmdState::NORMAL;
            return;
        }
        switch (value)
        {
        case ST7567A_SET_EV_CMD:
            _cmdState = CmdState::SET_EV;
            break;
        case ST7567A_SET_BOOSTER:
            _cmdState = CmdState::SET_BOOSTER;
            break;
        default:
            if ((value | ST7567A_SET_START_LINE_MASK) == (ST7567A_SET_START_LINE | ST7567A_SET_START_LINE_MASK))
            {
                _startLine = value & ST7567A_SET_START_LINE_MASK;
            }
            else if ((value | ST7567A_PAGE_ADDR_MASK) == (ST7567A_PAGE_ADDR | ST7567A_PAGE_ADDR_MASK))
            {
                _redrawState |= 1 << 0;
                _page = value & ST7567A_PAGE_ADDR_MASK;
            }
            else if ((value & ~0x1f) == 0)
            {
                if (value & 0x10)
                {
                    // _redrawState |= 1 << 1;
                    _column = (value << 4) | (_column & 0x0f);
                }
                else
                {
                    // _redrawState |= 1 << 2;
                    _column = (value & 0x0f) | (_column & 0xf0);
                }
            }
            break;
        }
    }
} // namespace app