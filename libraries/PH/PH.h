#ifndef PH_h
#define PH_h

#include "application.h"

class PH{
    public:
        PH();
        float read();
        void init();
    private:
        String _data;
        float _last;
};

#endif
