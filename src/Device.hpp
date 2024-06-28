#pragma once

#include <functional>
#include "PeriphWrappers.hpp"
#include "RingBuffer.hpp"
#include "SH1106.hpp"

namespace app {

struct DispRxFrame
{
    uint8_t data;
    bool isCommand;
    bool chipSelect;
    bool reset;
};

struct Device {
private:
    struct _PImpl;
    _PImpl & _impl;
public:

    GPIO led;
    GPIO button;
    SH1106 display;
    RingBuffer<uint16_t, 1024> displayRxBuf;

    using RxCallback = std::function<void()>;


    static Device& instance();

    void handleInterruptInputSPI();
    void handleInterruptInputSPI_CS();
    void inputSPIReceiveIT();
    void registerRxCallback(const RxCallback& cb) {
        _rxCallback = cb;
    }

private:
    Device();


    RxCallback _rxCallback;


};

} // namespace app


