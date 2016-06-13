#ifndef AtlasScientific_h
#define AtlasScientific_h
#define BAUD 9600

#define PIN0 D6
#define PIN1 D7

#define PH_CHANNEL 0
#define DO_CHANNEL 1
#define EC_CHANNEL 2
#define ORP_CHANNEL 3

#include "application.h"

class AtlasScientific{
	public:
		AtlasScientific(bool ph_active, bool ec_active, bool do_active, bool orp_active);
		void command(int channel, String command);
        bool data_available();
		float read(int channel);
		void init();
        void next();
	private:
		String _data;
		bool active_channels[4];
        float last_values[4];
        int _current_channel;
        bool running;
		void _open_channel(int channel);
        void _open_next_channel();
        void parse_values(String command);
};

#endif
