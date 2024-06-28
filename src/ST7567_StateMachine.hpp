#pragma once

#include <array>
#include <cstdint>
#include <cstddef>


namespace app {
struct ST7567_StateMachine {
    
public:
    
    static constexpr size_t W = 128;
    static constexpr size_t H = 64;
    std::array<uint8_t, W*H/8> buffer;


    ST7567_StateMachine() {
        buffer.fill(0);
    }
    void process(uint8_t value, bool dc);
    void display();
    bool updated() {
        return _updated;
    }
    void clearUpdated() {
        _updated = false;
    }

private:
    void _processData(uint8_t value);
    void _processCMD(uint8_t value);
    void _setPixel(uint8_t x, uint8_t y, bool value);
    size_t correctionCounter{0};

    enum class CmdState: uint8_t {
        NORMAL,
        SET_EV,
        SET_BOOSTER,
    };

    CmdState _cmdState{CmdState::NORMAL};


    uint8_t _redrawState{0};
    uint8_t _column{0};
    uint8_t _page{0};
    uint8_t _startLine{0};
    bool _updated{false};
};
} // namespace app