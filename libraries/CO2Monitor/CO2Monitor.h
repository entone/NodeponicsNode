#ifndef CO2Monitor_h
#define CO2Monitor_h

#include "application.h"

const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};

class CO2Monitor{
    public:
        CO2Monitor();
        int read();
        void init();
};

#endif
