#pragma once

#include <cstdint>






namespace app {


struct SPI;
struct GPIO;



class DisplayInterface {
public:
    DisplayInterface(SPI * spi, GPIO * cs);

    void write(uint8_t data);
    void write(const void * data, uint16_t size);
    

    void writeTransaction(const void * data, uint16_t size);


    void beginTransaction();
    void endTransaction();

    class Transaction { //< RAII for transaction
    public:
        Transaction(DisplayInterface& d): _d(d) {
            _d.beginTransaction();
        }
        ~Transaction() {
            _d.endTransaction();
        }
    private:
        DisplayInterface& _d;
    };

private:
    SPI * _spi;
    GPIO * _cs;
};

}