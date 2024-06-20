#pragma once


#include "SharpMemLCD.hpp"
#include "PeriphWrappers.hpp"

namespace app {

struct Device {
private:
    Device();

    struct _PImpl;
    _PImpl & _impl;
public:

    SharpMemLCD display;
    GPIO led;
    GPIO button;


    static Device& instance();


};

} // namespace app


