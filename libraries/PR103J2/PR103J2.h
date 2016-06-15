#ifndef PR103J2_h
#define PR103J2_h

#define NUM_READS 10

#include "application.h"

class PR103J2{
    public:
        PR103J2(uint8_t pin);
        float read();
        void init();
    private:
        bool _started;
        int _pin;
        int _index;
        int _avg[NUM_READS];
        float _total;
};

#endif
