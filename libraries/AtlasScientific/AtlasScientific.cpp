#include "application.h"
#include "AtlasScientific.h"

AtlasScientific::AtlasScientific(bool ph_active, bool do_active, bool ec_active, bool orp_active){
	active_channels[PH_CHANNEL] = ph_active;
    active_channels[DO_CHANNEL] = do_active;
	active_channels[EC_CHANNEL] = ec_active;
	active_channels[ORP_CHANNEL] = orp_active;
    _current_channel = 0;
    last_values[4] = {0.00};
}

void AtlasScientific::init(){
	Serial1.begin(BAUD);
    pinMode(PIN0, OUTPUT);
    pinMode(PIN1, OUTPUT);
    for(int i=0; i<4; i++){
        if(active_channels[i]){
            command(i, "RESPONSE,0");
            delay(1000);
            command(i, "C,1");
            delay(1000);
        }
    }
}

bool AtlasScientific::data_available(){
    while(Serial1.available()){
        char in = (char)Serial1.read();
        _data+= in;
        if(in == '\r'){
            Serial.print("Data Ended: "); Serial.println(_data);
            if(!_data.startsWith("*")) last_values[_current_channel] = _data.toFloat();
            _data = "";
            _open_next_channel();
            return true;
        }
        if(in == -1){
            Serial.print("Bad Data"); Serial.println(_data);
            _data = "";
            return false;
        }
    }
    return false;
}

void AtlasScientific::parse_values(String command){
    int numArgs = 0;
  	int beginIdx = 0;
  	int idx = command.indexOf(",");
  	String arg;
  	char charBuffer[32];
	while (idx != -1){
		arg = command.substring(beginIdx, idx);
		arg.toCharArray(charBuffer, 32);
		// add error handling for atoi:
		last_values[_current_channel] = atof(charBuffer);
		beginIdx = idx + 1;
		idx = command.indexOf(",", beginIdx);
	}
	arg = command.substring(beginIdx);
	arg.toCharArray(charBuffer, 32);
	last_values[_current_channel] = atof(charBuffer);
}

void AtlasScientific::_open_next_channel(){
	_current_channel+=1;
	if(_current_channel < 4 && active_channels[_current_channel]){
		_open_channel(_current_channel);
	}
	if(_current_channel < 4 && !active_channels[_current_channel]){
		_open_next_channel();
	}
	if(_current_channel >= 4){
		_current_channel = 0;
		_open_channel(_current_channel);
	}
	return;
}

void AtlasScientific::_open_channel(int channel){
	switch(channel){
		case PH_CHANNEL:
			digitalWrite(PIN0, LOW);
			digitalWrite(PIN1, LOW);
			break;
		case DO_CHANNEL:
			digitalWrite(PIN0, HIGH);
			digitalWrite(PIN1, LOW);
			break;
		case EC_CHANNEL:
			digitalWrite(PIN0, LOW);
			digitalWrite(PIN1, HIGH);
			break;
		case ORP_CHANNEL:
			digitalWrite(PIN0, HIGH);
			digitalWrite(PIN1, HIGH);
			break;
	}
    return;
}

void AtlasScientific::command(int channel, String command){
    Serial.print("Opening Channel:"); Serial.println(channel);
	_open_channel(channel);
    Serial.print("Running Command:"); Serial.println(command);
	Serial1.print(command);
    Serial1.print("\r");
}

float AtlasScientific::read(int channel){
    return last_values[channel];
}

void AtlasScientific::next(){
    _open_next_channel();
}
