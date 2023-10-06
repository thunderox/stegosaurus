
#include "DistrhoPlugin.hpp"
#include "voice.h"
#include <string.h>
#include <string>
#include <iostream>
#include <math.h>
#include <sstream> 
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>

using namespace std;

START_NAMESPACE_DISTRHO

const int max_voices = 4;

float buf0,buf1;
float f, pc, q;             //filter coefficients
float bf0, bf1, bf2, bf3, bf4;  //filter buffers (beware denormals!)
float t1, t2;              //temporary buffers
float selectivity, gain1, gain2, ratio, cap; 

class stegosaurus : public Plugin
{
	public:
	
	float fParameters[kParameterCount];
	double srate;
	int keys[128];
	voice voices[max_voices];
	vector <wavetable> wavetables;

	stegosaurus() : Plugin(kParameterCount, 0, 0)
	{
		std::memset(fParameters, 0, sizeof(float)*kParameterCount);
		srate = getSampleRate();
		for (int x=0; x<128; x++) { keys[x] = -1; }
		fParameters[kParameterBufferSize] = getBufferSize();
		
		for (int x=0; x<max_voices; x++)
		{
			voices[x].init(srate);
		}
	}

	const char* getDescription() const override
	{
		return "ThunderOx Stegosaurus - Drum Synthesizer Plugin";
	}
	
	const char* getHomePage() const override
	{
		return "https://thunderox.com/synths/stegosaurus";
	}

	const char* getLabel() const
	{
		return "stegosaurus";
	}

	const char* getMaker() const
	{
		return "Nick S. Bailey";
	}

	const char* getLicense() const
	{
		return "GPL2";
	}

	//Get the plugin version, in hexadecimal.

	uint32_t getVersion() const override
	{
		return d_version(1, 0, 0);
	}

	int64_t getUniqueId() const
	{
		return d_cconst('T','O','S','S');
	}
		
		
 	void initParameter (uint32_t index, Parameter& parameter) override
 	{
		switch (index)
		{
			// MASTER CONTROLS -----------------------------
											
			case stegosaurus_VOLUME:
				parameter.name = "Volume";
				parameter.symbol = "stegosaurus_VOLUME";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 2.0f;
				fParameters[stegosaurus_VOLUME] = parameter.ranges.def;
				break;
				
			// KICK DRUM ------------------------------------
			
			// OSCILLATOR ONE
						
			case stegosaurus_KICK_OSC1_ACTIVE:
				parameter.name = "stegosaurus_KICK_OSC1_ACTIVE";
				parameter.symbol = "stegosaurus_KICK_OSC1_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_NOISE:
				parameter.name = "stegosaurus_KICK_OSC1_NOISE";
				parameter.symbol = "stegosaurus_KICK_OSC1_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_PITCH:
				parameter.name = "stegosaurus_KICK_OSC1_PITCH";
				parameter.symbol = "stegosaurus_KICK_OSC1_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_AMP_ATTACK:
				parameter.name = "stegosaurus_KICK_OSC1_AMP_ATTACK";
				parameter.symbol = "stegosaurus_KICK_OSC1_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_AMP_DECAY:
				parameter.name = "stegosaurus_KICK_OSC1_AMP_DECAY";
				parameter.symbol = "stegosaurus_KICK_OSC1_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_AMP_SUSTAIN:
				parameter.name = "stegosaurus_KICK_OSC1_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_KICK_OSC1_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_AMP_RELEASE:
				parameter.name = "stegosaurus_KICK_OSC1_AMP_RELEASE";
				parameter.symbol = "stegosaurus_KICK_OSC1_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_PITCH_ATTACK:
				parameter.name = "stegosaurus_KICK_OSC1_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_KICK_OSC1_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_PITCH_DECAY:
				parameter.name = "stegosaurus_KICK_OSC1_PITCH_DECAY";
				parameter.symbol = "stegosaurus_KICK_OSC1_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_KICK_OSC1_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_KICK_OSC1_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_PITCH_RELEASE:
				parameter.name = "stegosaurus_KICK_OSC1_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_KICK_OSC1_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC1_VOLUME:
				parameter.name = "stegosaurus_KICK_OSC1_VOLUME";
				parameter.symbol = "stegosaurus_KICK_OSC1_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC1_VOLUME] = parameter.ranges.def;
				break;
				
			// OSCILLATOR TWO
						
			case stegosaurus_KICK_OSC2_ACTIVE:
				parameter.name = "stegosaurus_KICK_OSC2_ACTIVE";
				parameter.symbol = "stegosaurus_KICK_OSC2_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_NOISE:
				parameter.name = "stegosaurus_KICK_OSC2_NOISE";
				parameter.symbol = "stegosaurus_KICK_OSC2_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_PITCH:
				parameter.name = "stegosaurus_KICK_OSC2_PITCH";
				parameter.symbol = "stegosaurus_KICK_OSC2_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_AMP_ATTACK:
				parameter.name = "stegosaurus_KICK_OSC2_AMP_ATTACK";
				parameter.symbol = "stegosaurus_KICK_OSC2_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_AMP_DECAY:
				parameter.name = "stegosaurus_KICK_OSC2_AMP_DECAY";
				parameter.symbol = "stegosaurus_KICK_OSC2_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_AMP_SUSTAIN:
				parameter.name = "stegosaurus_KICK_OSC2_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_KICK_OSC2_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_AMP_RELEASE:
				parameter.name = "stegosaurus_KICK_OSC2_AMP_RELEASE";
				parameter.symbol = "stegosaurus_KICK_OSC2_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_PITCH_ATTACK:
				parameter.name = "stegosaurus_KICK_OSC2_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_KICK_OSC2_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_PITCH_DECAY:
				parameter.name = "stegosaurus_KICK_OSC2_PITCH_DECAY";
				parameter.symbol = "stegosaurus_KICK_OSC2_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_KICK_OSC2_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_KICK_OSC2_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_PITCH_RELEASE:
				parameter.name = "stegosaurus_KICK_OSC2_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_KICK_OSC2_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_KICK_OSC2_VOLUME:
				parameter.name = "stegosaurus_KICK_OSC2_VOLUME";
				parameter.symbol = "stegosaurus_KICK_OSC2_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_KICK_OSC2_VOLUME] = parameter.ranges.def;
				break;
				
			// SNARE DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_SNARE_OSC1_ACTIVE:
				parameter.name = "stegosaurus_SNARE_OSC1_ACTIVE";
				parameter.symbol = "stegosaurus_SNARE_OSC1_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_NOISE:
				parameter.name = "stegosaurus_SNARE_OSC1_NOISE";
				parameter.symbol = "stegosaurus_SNARE_OSC1_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_PITCH:
				parameter.name = "stegosaurus_SNARE_OSC1_PITCH";
				parameter.symbol = "stegosaurus_SNARE_OSC1_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_AMP_ATTACK:
				parameter.name = "stegosaurus_SNARE_OSC1_AMP_ATTACK";
				parameter.symbol = "stegosaurus_SNARE_OSC1_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_AMP_DECAY:
				parameter.name = "stegosaurus_SNARE_OSC1_AMP_DECAY";
				parameter.symbol = "stegosaurus_SNARE_OSC1_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_AMP_SUSTAIN:
				parameter.name = "stegosaurus_SNARE_OSC1_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_SNARE_OSC1_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_AMP_RELEASE:
				parameter.name = "stegosaurus_SNARE_OSC1_AMP_RELEASE";
				parameter.symbol = "stegosaurus_SNARE_OSC1_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_PITCH_ATTACK:
				parameter.name = "stegosaurus_SNARE_OSC1_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_SNARE_OSC1_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_PITCH_DECAY:
				parameter.name = "stegosaurus_SNARE_OSC1_PITCH_DECAY";
				parameter.symbol = "stegosaurus_SNARE_OSC1_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_SNARE_OSC1_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_SNARE_OSC1_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_PITCH_RELEASE:
				parameter.name = "stegosaurus_SNARE_OSC1_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_SNARE_OSC1_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC1_VOLUME:
				parameter.name = "stegosaurus_SNARE_OSC1_VOLUME";
				parameter.symbol = "stegosaurus_SNARE_OSC1_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC1_VOLUME] = parameter.ranges.def;
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_SNARE_OSC2_ACTIVE:
				parameter.name = "stegosaurus_SNARE_OSC2_ACTIVE";
				parameter.symbol = "stegosaurus_SNARE_OSC2_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_NOISE:
				parameter.name = "stegosaurus_SNARE_OSC2_NOISE";
				parameter.symbol = "stegosaurus_SNARE_OSC2_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_PITCH:
				parameter.name = "stegosaurus_SNARE_OSC2_PITCH";
				parameter.symbol = "stegosaurus_SNARE_OSC2_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_AMP_ATTACK:
				parameter.name = "stegosaurus_SNARE_OSC2_AMP_ATTACK";
				parameter.symbol = "stegosaurus_SNARE_OSC2_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_AMP_DECAY:
				parameter.name = "stegosaurus_SNARE_OSC2_AMP_DECAY";
				parameter.symbol = "stegosaurus_SNARE_OSC2_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_AMP_SUSTAIN:
				parameter.name = "stegosaurus_SNARE_OSC2_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_SNARE_OSC2_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_AMP_RELEASE:
				parameter.name = "stegosaurus_SNARE_OSC2_AMP_RELEASE";
				parameter.symbol = "stegosaurus_SNARE_OSC2_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_PITCH_ATTACK:
				parameter.name = "stegosaurus_SNARE_OSC2_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_SNARE_OSC2_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_PITCH_DECAY:
				parameter.name = "stegosaurus_SNARE_OSC2_PITCH_DECAY";
				parameter.symbol = "stegosaurus_SNARE_OSC2_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_SNARE_OSC2_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_SNARE_OSC2_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_PITCH_RELEASE:
				parameter.name = "stegosaurus_SNARE_OSC2_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_SNARE_OSC2_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_SNARE_OSC2_VOLUME:
				parameter.name = "stegosaurus_SNARE_OSC2_VOLUME";
				parameter.symbol = "stegosaurus_SNARE_OSC2_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_SNARE_OSC2_VOLUME] = parameter.ranges.def;
				break;
				
			// CLOSED HAT DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_CLHAT_OSC1_ACTIVE:
				parameter.name = "stegosaurus_CLHAT_OSC1_ACTIVE";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_NOISE:
				parameter.name = "stegosaurus_CLHAT_OSC1_NOISE";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_PITCH:
				parameter.name = "stegosaurus_CLHAT_OSC1_PITCH";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_AMP_ATTACK:
				parameter.name = "stegosaurus_CLHAT_OSC1_AMP_ATTACK";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_AMP_DECAY:
				parameter.name = "stegosaurus_CLHAT_OSC1_AMP_DECAY";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_AMP_SUSTAIN:
				parameter.name = "stegosaurus_CLHAT_OSC1_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_AMP_RELEASE:
				parameter.name = "stegosaurus_CLHAT_OSC1_AMP_RELEASE";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_PITCH_ATTACK:
				parameter.name = "stegosaurus_CLHAT_OSC1_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_PITCH_DECAY:
				parameter.name = "stegosaurus_CLHAT_OSC1_PITCH_DECAY";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_PITCH_RELEASE:
				parameter.name = "stegosaurus_CLHAT_OSC1_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC1_VOLUME:
				parameter.name = "stegosaurus_CLHAT_OSC1_VOLUME";
				parameter.symbol = "stegosaurus_CLHAT_OSC1_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC1_VOLUME] = parameter.ranges.def;
				break;

			// OSCILLATOR TWO
				
			case stegosaurus_CLHAT_OSC2_ACTIVE:
				parameter.name = "stegosaurus_CLHAT_OSC2_ACTIVE";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_NOISE:
				parameter.name = "stegosaurus_CLHAT_OSC2_NOISE";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_PITCH:
				parameter.name = "stegosaurus_CLHAT_OSC2_PITCH";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_AMP_ATTACK:
				parameter.name = "stegosaurus_CLHAT_OSC2_AMP_ATTACK";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_AMP_DECAY:
				parameter.name = "stegosaurus_CLHAT_OSC2_AMP_DECAY";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_AMP_SUSTAIN:
				parameter.name = "stegosaurus_CLHAT_OSC2_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_AMP_RELEASE:
				parameter.name = "stegosaurus_CLHAT_OSC2_AMP_RELEASE";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_PITCH_ATTACK:
				parameter.name = "stegosaurus_CLHAT_OSC2_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_PITCH_DECAY:
				parameter.name = "stegosaurus_CLHAT_OSC2_PITCH_DECAY";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_PITCH_RELEASE:
				parameter.name = "stegosaurus_CLHAT_OSC2_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_CLHAT_OSC2_VOLUME:
				parameter.name = "stegosaurus_CLHAT_OSC2_VOLUME";
				parameter.symbol = "stegosaurus_CLHAT_OSC2_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_CLHAT_OSC2_VOLUME] = parameter.ranges.def;
				break;


			// OPEN HAT DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_OPHAT_OSC1_ACTIVE:
				parameter.name = "stegosaurus_OPHAT_OSC1_ACTIVE";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_NOISE:
				parameter.name = "stegosaurus_OPHAT_OSC1_NOISE";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_PITCH:
				parameter.name = "stegosaurus_OPHAT_OSC1_PITCH";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_AMP_ATTACK:
				parameter.name = "stegosaurus_OPHAT_OSC1_AMP_ATTACK";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_AMP_DECAY:
				parameter.name = "stegosaurus_OPHAT_OSC1_AMP_DECAY";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_AMP_SUSTAIN:
				parameter.name = "stegosaurus_OPHAT_OSC1_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_AMP_RELEASE:
				parameter.name = "stegosaurus_OPHAT_OSC1_AMP_RELEASE";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_PITCH_ATTACK:
				parameter.name = "stegosaurus_OPHAT_OSC1_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_PITCH_DECAY:
				parameter.name = "stegosaurus_OPHAT_OSC1_PITCH_DECAY";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_PITCH_RELEASE:
				parameter.name = "stegosaurus_OPHAT_OSC1_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC1_VOLUME:
				parameter.name = "stegosaurus_OPHAT_OSC1_VOLUME";
				parameter.symbol = "stegosaurus_OPHAT_OSC1_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC1_VOLUME] = parameter.ranges.def;
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_OPHAT_OSC2_ACTIVE:
				parameter.name = "stegosaurus_OPHAT_OSC2_ACTIVE";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_ACTIVE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_ACTIVE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_NOISE:
				parameter.name = "stegosaurus_OPHAT_OSC2_NOISE";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_NOISE";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_NOISE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_PITCH:
				parameter.name = "stegosaurus_OPHAT_OSC2_PITCH";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_PITCH";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_PITCH] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_AMP_ATTACK:
				parameter.name = "stegosaurus_OPHAT_OSC2_AMP_ATTACK";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_AMP_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_AMP_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_AMP_DECAY:
				parameter.name = "stegosaurus_OPHAT_OSC2_AMP_DECAY";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_AMP_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_AMP_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_AMP_SUSTAIN:
				parameter.name = "stegosaurus_OPHAT_OSC2_AMP_SUSTAIN";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_AMP_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_AMP_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_AMP_RELEASE:
				parameter.name = "stegosaurus_OPHAT_OSC2_AMP_RELEASE";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_AMP_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_AMP_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_PITCH_ATTACK:
				parameter.name = "stegosaurus_OPHAT_OSC2_PITCH_ATTACK";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_PITCH_ATTACK";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_PITCH_ATTACK] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_PITCH_DECAY:
				parameter.name = "stegosaurus_OPHAT_OSC2_PITCH_DECAY";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_PITCH_DECAY";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_PITCH_DECAY] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN:
				parameter.name = "stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_PITCH_RELEASE:
				parameter.name = "stegosaurus_OPHAT_OSC2_PITCH_RELEASE";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_PITCH_RELEASE";
				parameter.ranges.def = 1.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_PITCH_RELEASE] = parameter.ranges.def;
				break;
				
			case stegosaurus_OPHAT_OSC2_VOLUME:
				parameter.name = "stegosaurus_OPHAT_OSC2_VOLUME";
				parameter.symbol = "stegosaurus_OPHAT_OSC2_VOLUME";
				parameter.ranges.def = 0.0f;
				parameter.ranges.min = 0.0f;
				parameter.ranges.max = 1.0f;
				fParameters[stegosaurus_OPHAT_OSC2_VOLUME] = parameter.ranges.def;
				break;
		}
	}
   
	float getParameterValue(uint32_t index) const override
	{
		return fParameters[index];
	}

	void setParameterValue(uint32_t index, float value) override
	{
		fParameters[index] = value;
	}
	
	//----------------------------------------------
		 				
	void initProgramName(uint32_t index, String& program_name) override
	{
	}

    
	void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
	{
		
		float *const out_left1 = outputs[0];
		float *const out_right1 = outputs[1];
		
		float *const out_left2 = outputs[2];
		float *const out_right2 = outputs[3];
		
		float *const out_left3 = outputs[4];
		float *const out_right3 = outputs[5];
		
		float *const out_left4 = outputs[6];
		float *const out_right4 = outputs[7];
		
		for (uint32_t i = 0; i < frames; i++)
		{
       
	        }
	}
	
	//-------------------------------------------------------------------------------------------------------



	private:

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(stegosaurus);
};


Plugin *createPlugin()
{

	stegosaurus* new_stegosaurus = new stegosaurus();

	// LOAD WAVETABLES;

	string user_path = getenv("USER");
	stringstream wave_path;
	stringstream wave_file;
	struct dirent *d;
	struct stat st;
	long length;

	DIR *dr;
	wave_path.str("");
	wave_path << "/home/" << user_path << "/.lv2/stegosaurus.lv2/waves";
	
	dr = opendir(wave_path.str().c_str());
		 
	if (dr != NULL)
	{
		for( d=readdir(dr); d!=NULL; d=readdir(dr)) // List all files here
		{
			wave_file.str("");
			wave_file << wave_path.str() << "/" << d->d_name;

			FILE* fp = fopen (wave_file.str().c_str(),"r");
			fseek(fp, 0, SEEK_END); // We can use rewind(fp); also
			
			if (ftell(fp) == 35360 )
			{					
				length = (ftell(fp) - 80 )/ 4;
				
				wavetable new_waveform;
					
				float* source_waveform_buffer = (float *)malloc(length*sizeof(float)) ;
				new_waveform.buffer = (float *)malloc((12*length)*sizeof(float)) ;
							
				// LOAD RAW WAVEFORM
							
				fseek(fp, 80, SEEK_SET);
				fread(source_waveform_buffer ,1, length*sizeof(float), fp);
				fclose(fp);	
								
				//------ FILTER 8 DIFFERENT VERSINS OF WAVEFORM TO REDUCE ALIASING

				// Moog 24 dB/oct resonant lowpass VCF
				// References: CSound source code, Stilson/Smith CCRMA paper.
				// Modified by paul.kellett@maxim.abel.co.uk July 2000

				// Set coefficients given frequency & resonance [0.0...1.0]
				
				float frequency = 0.8;
				float resonance = 0;	
				float in;

				for (int wave=1; wave<12; wave++)
				{
				
					float minpeak = 0;
					float maxpeak = 0;
					float maxvol = 0;
					
					in = 0;
					
					for (int x=0; x<length; x++)
					{
						in = source_waveform_buffer[x];

						q = 1.0f - frequency;
						pc = frequency + 0.8f * frequency * q;
						f = pc + pc - 1.0f;
						q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
				
						// Filter (in [-1.0...+1.0])

						in -= q * bf4;                          //feedback
						t1 = bf1;  bf1 = (in + bf0) * pc - bf1 * f;
						t2 = bf2;  bf2 = (bf1 + t1) * pc - bf2 * f;
						t1 = bf3;  bf3 = (bf2 + t2) * pc - bf3 * f;
						bf4 = (bf3 + t1) * pc - bf4 * f;
						bf4 = bf4 - bf4 * bf4 * bf4 * 0.166667f;    //clipping
						bf0 = in;

						new_waveform.buffer[x + (wave*length)] = bf4;
							
						if (bf4 < 0 && bf4 < minpeak) minpeak = bf4;
						if (bf4 > 0 && bf4 > maxpeak) maxpeak = bf4;
					
					}

					if (-minpeak > maxpeak) maxvol = -minpeak;
						else maxvol = maxpeak;

					if (-minpeak == maxpeak) maxvol = maxpeak;

					float amp = 0.8 / maxvol;

					for (int y=0; y<length; y++)	// Normalise
					{
						new_waveform.buffer[y+(wave*length)] = new_waveform.buffer[y+(wave*length)]*amp;
					}

					frequency /= 2.4;	
			}
				
			for (int x=0; x<length; x++)
			{
				new_waveform.buffer[x] = source_waveform_buffer[x] * 0.8;
			}		
			
			new_waveform.length = length;
			new_waveform.name = d->d_name;
			new_stegosaurus->wavetables.push_back(new_waveform);
			free(source_waveform_buffer);
			}
		}	
	}	

	return new_stegosaurus;
}

END_NAMESPACE_DISTRHO











