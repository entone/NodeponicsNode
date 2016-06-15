#include "application.h"
#include "PR103J2.h"
#include "math.h"

PR103J2::PR103J2(uint8_t pin){
    _pin = pin;
    _total = 0;
    _index = 0;
    _started = false;
}

void PR103J2::init(){
    pinMode(_pin, INPUT);
}

float PR103J2::read(){
    //if(_index >= NUM_READS){
    //    if(!_started) _started = true;
    //    _index = 0;
    //}
    int val = analogRead(_pin);
    //Serial.print("PR103J2: "); Serial.println(val);
    float R = 10000/(4095.0/val-1.0); // use a 10K ohm resistor with the thermistor
    //Serial.print("PR103J2: "); Serial.println(R);

    // Get temperature from Steinhart equation (US sensors thermistor, 10K, B = 3892) *****************************************
    float A = 0.00113929600457259;
    float B = 0.000231949467390149;
    float C = 0.000000105992476218967;
    float D = -0.0000000000667898975192618;
    float E = log(R);

    // calculate temperature in Kelvin
    float T = 1/(A + (B*E) + (C*(E*E*E)) + (D*(E*E*E*E*E)));
    //Serial.print("Kelvin: "); Serial.println(T);
    delay(1); // float calcs are processor intensive for 8-bit processors; add some delay
    return T-273.15;

    //_total-= _avg[_index];
    //_avg[_index] = Cel;
    //_total+= Cel;
    //_index++;
    //Serial.print("Cel: "); Serial.println(_total/NUM_READS);
    //return Cel;
    //return _total/NUM_READS;
}
