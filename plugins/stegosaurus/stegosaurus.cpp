
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

const int number_of_voices = 4;

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
	voice voices[number_of_voices];
	vector <wavetable> wavetables;
	noise noise1;

	stegosaurus() : Plugin(kParameterCount, 0, 0)
	{
		std::memset(fParameters, 0, sizeof(float)*kParameterCount);
		srate = getSampleRate();
		for (int x=0; x<128; x++) { keys[x] = -1; }
		fParameters[kParameterBufferSize] = getBufferSize();
		
		for (int x=0; x<number_of_voices; x++)
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

    	//----------------------------------------------	//----------------------------------------------
    	
	void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
	{
		
		float *const out_left1 = outputs[0];
		float *const out_right1 = outputs[1];
		
		memset( out_left1, 0, sizeof(double)*(frames*0.5) );
		memset( out_right1, 0, sizeof(double)*(frames*0.5) );
		
		float *const out_left2 = outputs[2];
		float *const out_right2 = outputs[3];
		
		memset( out_left2, 0, sizeof(double)*(frames*0.5) );
		memset( out_right2, 0, sizeof(double)*(frames*0.5) );
		
		//----------------------------------------------------------------------------------------
		
		for (uint32_t i=0; i<midiEventCount; ++i)
		{		
			const uint8_t* ev = midiEvents[i].data;
			int midi_command = (int)ev[0];
			int midi_note = (int)ev[1];
			int midi_volume = (int)ev[2];
			
			// CHECK FOR NOTE ON - OMNI MODE SOME PEOPLE PREFER 10 BUT I WANT IT ALL GRRR!
			
		      	if ( midi_command >= 0x90 && midi_command <= 0x9f && midi_volume > 0)
			{
				if (midi_note == 60)
				{
					cout << "Kick triggered" << endl;
					
					voices[0].active = true;
					voices[0].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[0].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[0].adsr_osc1_amp.level = 0;
					voices[0].adsr_osc1_pitch.level = 0;
					voices[0].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[0].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[0].adsr_osc2_amp.level = 0;
					voices[0].adsr_osc2_pitch.level = 0;
					voices[0].frequency = midi_note;
					voices[0].volume = (float)midi_volume/128 ;
					keys[midi_note] = 0;
				}
		
				if (midi_note == 62)
				{
					cout << "Snare triggered" << endl;
					
					voices[1].active = true;
					voices[1].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[1].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[1].adsr_osc1_amp.level = 0;
					voices[1].adsr_osc1_pitch.level = 0;
					voices[1].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[1].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[1].adsr_osc2_amp.level = 0;
					voices[1].adsr_osc2_pitch.level = 0;
					voices[1].frequency = midi_note;
					voices[1].volume = (float)midi_volume/128 ;
					keys[midi_note] = 1;
				}
				
				if (midi_note == 64)
				{
					cout << "Closed Hat triggered" << endl;
					
					voices[2].active = true;
					voices[2].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[2].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[2].adsr_osc1_amp.level = 0;
					voices[2].adsr_osc1_pitch.level = 0;
					voices[2].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[2].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[2].adsr_osc2_amp.level = 0;
					voices[2].adsr_osc2_pitch.level = 0;
					voices[2].frequency = midi_note;
					voices[2].volume = (float)midi_volume/128 ;
					keys[midi_note] = 2;
					
					voices[3].adsr_osc1_amp.state = ENV_STATE_DORMANT;
					voices[3].adsr_osc1_pitch.state = ENV_STATE_DORMANT;
					voices[3].adsr_osc2_amp.state = ENV_STATE_DORMANT;
					voices[3].adsr_osc2_pitch.state = ENV_STATE_DORMANT;
				}
				
				if (midi_note == 65)
				{
					cout << "Open Hat triggered" << endl;
					
					voices[3].active = true;
					voices[3].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[3].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[3].adsr_osc1_amp.level = 0;
					voices[3].adsr_osc1_pitch.level = 0;
					voices[3].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[3].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[3].adsr_osc2_amp.level = 0;
					voices[3].adsr_osc2_pitch.level = 0;
					voices[3].frequency = midi_note;
					voices[3].volume = (float)midi_volume/128 ;
					keys[midi_note] = 3;
					
					voices[2].adsr_osc1_amp.state = ENV_STATE_DORMANT;
					voices[2].adsr_osc1_pitch.state = ENV_STATE_DORMANT;
					voices[2].adsr_osc2_amp.state = ENV_STATE_DORMANT;
					voices[2].adsr_osc2_pitch.state = ENV_STATE_DORMANT;
				}
			}
			
			// CHECK FOR NOTE OFF - OMNI MODE
			
		      	if ( midi_command >= 0x80 && midi_command <= 0x8f
				||  midi_command >= 0x90 && midi_command <= 0x9f && midi_volume == 0)
			{
				if (midi_note == 60)
				{
					voices[0].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 62)
				{
					voices[1].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 64)
				{
					voices[2].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 65)
				{
					voices[3].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
			}
			
		}	
		//----------------------------------------------------------------------------------------
		
		voices[0].osc[0].active = fParameters[stegosaurus_KICK_OSC1_ACTIVE];
		voices[0].osc[1].active = fParameters[stegosaurus_KICK_OSC2_ACTIVE];
		voices[1].osc[0].active = fParameters[stegosaurus_SNARE_OSC1_ACTIVE];
		voices[1].osc[1].active = fParameters[stegosaurus_SNARE_OSC2_ACTIVE];
		voices[2].osc[0].active = fParameters[stegosaurus_CLHAT_OSC1_ACTIVE];
		voices[2].osc[1].active = fParameters[stegosaurus_CLHAT_OSC2_ACTIVE];
		voices[3].osc[0].active = fParameters[stegosaurus_OPHAT_OSC1_ACTIVE];
		voices[3].osc[1].active = fParameters[stegosaurus_OPHAT_OSC2_ACTIVE];
		
		float osc1_pitch[4];
		float osc1_volume[4];
		float osc1_noise[4];
		
		float osc2_pitch[4];
		float osc2_volume[4];
		float osc2_noise[4];

		osc1_volume[0] = fParameters[stegosaurus_KICK_OSC1_VOLUME];
		osc1_pitch[0] = fParameters[stegosaurus_KICK_OSC1_PITCH];
		osc1_noise[0] = fParameters[stegosaurus_KICK_OSC1_NOISE];

		osc2_volume[0] = fParameters[stegosaurus_KICK_OSC2_VOLUME];
		osc2_pitch[0] = fParameters[stegosaurus_KICK_OSC2_PITCH];
		osc2_noise[0] = fParameters[stegosaurus_KICK_OSC2_NOISE];
		
		osc1_volume[1] = fParameters[stegosaurus_SNARE_OSC1_VOLUME];
		osc1_pitch[1] = fParameters[stegosaurus_SNARE_OSC1_PITCH];
		osc1_noise[1] = fParameters[stegosaurus_SNARE_OSC1_NOISE];

		osc2_volume[1] = fParameters[stegosaurus_SNARE_OSC2_VOLUME];
		osc2_pitch[1] = fParameters[stegosaurus_SNARE_OSC2_PITCH];
		osc2_noise[1] = fParameters[stegosaurus_SNARE_OSC2_NOISE];
		
		osc1_volume[2] = fParameters[stegosaurus_CLHAT_OSC1_VOLUME];
		osc1_pitch[2] = fParameters[stegosaurus_CLHAT_OSC1_PITCH];
		osc1_noise[2] = fParameters[stegosaurus_CLHAT_OSC1_NOISE];

		osc2_volume[2] = fParameters[stegosaurus_CLHAT_OSC2_VOLUME];
		osc2_pitch[2] = fParameters[stegosaurus_CLHAT_OSC2_PITCH];
		osc2_noise[2] = fParameters[stegosaurus_CLHAT_OSC2_NOISE];
		
		osc1_volume[3] = fParameters[stegosaurus_OPHAT_OSC1_VOLUME];
		osc1_pitch[3] = fParameters[stegosaurus_OPHAT_OSC1_PITCH];
		osc1_noise[3] = fParameters[stegosaurus_OPHAT_OSC1_NOISE];

		osc2_volume[3] = fParameters[stegosaurus_OPHAT_OSC2_VOLUME];
		osc2_pitch[3] = fParameters[stegosaurus_OPHAT_OSC2_PITCH];
		osc2_noise[3] = fParameters[stegosaurus_OPHAT_OSC2_NOISE];
				
		// OSCILATOR ONES PARAMS
		
		voices[0].adsr_osc1_amp.attack = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_ATTACK],10); 
		voices[1].adsr_osc1_amp.attack = fast_pow(fParameters[stegosaurus_SNARE_OSC1_AMP_ATTACK],10); 
		voices[2].adsr_osc1_amp.attack = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_AMP_ATTACK],10); 
		voices[3].adsr_osc1_amp.attack = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_AMP_ATTACK],10);
		
		voices[0].adsr_osc1_amp.decay = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_DECAY],10); 
		voices[1].adsr_osc1_amp.decay = fast_pow(fParameters[stegosaurus_SNARE_OSC1_AMP_DECAY],10); 
		voices[2].adsr_osc1_amp.decay = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_AMP_DECAY],10); 
		voices[3].adsr_osc1_amp.decay = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_AMP_DECAY],10);
		
		voices[0].adsr_osc1_amp.sustain = 1-fParameters[stegosaurus_KICK_OSC1_AMP_SUSTAIN]; 
		voices[1].adsr_osc1_amp.sustain = 1-fParameters[stegosaurus_SNARE_OSC1_AMP_SUSTAIN]; 
		voices[2].adsr_osc1_amp.sustain = 1-fParameters[stegosaurus_CLHAT_OSC1_AMP_SUSTAIN]; 
		voices[3].adsr_osc1_amp.sustain = 1-fParameters[stegosaurus_OPHAT_OSC1_AMP_SUSTAIN];
		
		voices[0].adsr_osc1_amp.release = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_RELEASE],10); 
		voices[1].adsr_osc1_amp.release = fast_pow(fParameters[stegosaurus_SNARE_OSC1_AMP_RELEASE],10); 
		voices[2].adsr_osc1_amp.release = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_AMP_RELEASE],10); 
		voices[3].adsr_osc1_amp.release = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_AMP_RELEASE],10);
		
		voices[0].adsr_osc1_pitch.attack = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_ATTACK],10); 
		voices[1].adsr_osc1_pitch.attack = fast_pow(fParameters[stegosaurus_SNARE_OSC1_PITCH_ATTACK],10); 
		voices[2].adsr_osc1_pitch.attack = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_PITCH_ATTACK],10); 
		voices[3].adsr_osc1_pitch.attack = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_PITCH_ATTACK],10);
		
		voices[0].adsr_osc1_pitch.decay = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_DECAY],10); 
		voices[1].adsr_osc1_pitch.decay = fast_pow(fParameters[stegosaurus_SNARE_OSC1_PITCH_DECAY],10); 
		voices[2].adsr_osc1_pitch.decay = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_PITCH_DECAY],10); 
		voices[3].adsr_osc1_pitch.decay = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_PITCH_DECAY],10);
		
		voices[0].adsr_osc1_pitch.sustain = 1-fParameters[stegosaurus_KICK_OSC1_PITCH_SUSTAIN]; 
		voices[1].adsr_osc1_pitch.sustain = 1-fParameters[stegosaurus_SNARE_OSC1_PITCH_SUSTAIN]; 
		voices[2].adsr_osc1_pitch.sustain = 1-fParameters[stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN]; 
		voices[3].adsr_osc1_pitch.sustain = 1-fParameters[stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN];
		
		voices[0].adsr_osc1_pitch.release = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_RELEASE],10); 
		voices[1].adsr_osc1_pitch.release = fast_pow(fParameters[stegosaurus_SNARE_OSC1_PITCH_RELEASE],10); 
		voices[2].adsr_osc1_pitch.release = fast_pow(fParameters[stegosaurus_CLHAT_OSC1_PITCH_RELEASE],10); 
		voices[3].adsr_osc1_pitch.release = fast_pow(fParameters[stegosaurus_OPHAT_OSC1_PITCH_RELEASE],10);
		
		voices[0].osc[0].frequency = fParameters[stegosaurus_KICK_OSC1_PITCH] * 128;
		voices[1].osc[0].frequency = fParameters[stegosaurus_SNARE_OSC1_PITCH] * 128;
		voices[2].osc[0].frequency = fParameters[stegosaurus_CLHAT_OSC1_PITCH] * 128;
		voices[3].osc[0].frequency = fParameters[stegosaurus_OPHAT_OSC1_PITCH] * 128;

				
		// OSCILATOR TWOS PARAMS
		
		voices[0].adsr_osc2_amp.attack = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_ATTACK],10); 
		voices[1].adsr_osc2_amp.attack = fast_pow(fParameters[stegosaurus_SNARE_OSC2_AMP_ATTACK],10); 
		voices[2].adsr_osc2_amp.attack = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_AMP_ATTACK],10); 
		voices[3].adsr_osc2_amp.attack = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_AMP_ATTACK],10);
		
		voices[0].adsr_osc2_amp.decay = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_DECAY],10); 
		voices[1].adsr_osc2_amp.decay = fast_pow(fParameters[stegosaurus_SNARE_OSC2_AMP_DECAY],10); 
		voices[2].adsr_osc2_amp.decay = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_AMP_DECAY],10); 
		voices[3].adsr_osc2_amp.decay = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_AMP_DECAY],10);
		
		voices[0].adsr_osc2_amp.sustain = 1-fParameters[stegosaurus_KICK_OSC2_AMP_SUSTAIN]; 
		voices[1].adsr_osc2_amp.sustain = 1-fParameters[stegosaurus_SNARE_OSC2_AMP_SUSTAIN]; 
		voices[2].adsr_osc2_amp.sustain = 1-fParameters[stegosaurus_CLHAT_OSC2_AMP_SUSTAIN]; 
		voices[3].adsr_osc2_amp.sustain = 1-fParameters[stegosaurus_OPHAT_OSC2_AMP_SUSTAIN];
		
		voices[0].adsr_osc2_amp.release = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_RELEASE],10); 
		voices[1].adsr_osc2_amp.release = fast_pow(fParameters[stegosaurus_SNARE_OSC2_AMP_RELEASE],10); 
		voices[2].adsr_osc2_amp.release = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_AMP_RELEASE],10); 
		voices[3].adsr_osc2_amp.release = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_AMP_RELEASE],10);
		
		voices[0].adsr_osc2_pitch.attack = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_ATTACK],10); 
		voices[1].adsr_osc2_pitch.attack = fast_pow(fParameters[stegosaurus_SNARE_OSC2_PITCH_ATTACK],10); 
		voices[2].adsr_osc2_pitch.attack = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_PITCH_ATTACK],10); 
		voices[3].adsr_osc2_pitch.attack = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_PITCH_ATTACK],10);
		
		voices[0].adsr_osc2_pitch.decay = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_DECAY],10); 
		voices[1].adsr_osc2_pitch.decay = fast_pow(fParameters[stegosaurus_SNARE_OSC2_PITCH_DECAY],10); 
		voices[2].adsr_osc2_pitch.decay = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_PITCH_DECAY],10); 
		voices[3].adsr_osc2_pitch.decay = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_PITCH_DECAY],10);
		
		voices[0].adsr_osc2_pitch.sustain = 1-fParameters[stegosaurus_KICK_OSC2_PITCH_SUSTAIN]; 
		voices[1].adsr_osc2_pitch.sustain = 1-fParameters[stegosaurus_SNARE_OSC2_PITCH_SUSTAIN]; 
		voices[2].adsr_osc2_pitch.sustain = 1-fParameters[stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN]; 
		voices[3].adsr_osc2_pitch.sustain = 1-fParameters[stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN];
		
		voices[0].adsr_osc2_pitch.release = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_RELEASE],10); 
		voices[1].adsr_osc2_pitch.release = fast_pow(fParameters[stegosaurus_SNARE_OSC2_PITCH_RELEASE],10); 
		voices[2].adsr_osc2_pitch.release = fast_pow(fParameters[stegosaurus_CLHAT_OSC2_PITCH_RELEASE],10); 
		voices[3].adsr_osc2_pitch.release = fast_pow(fParameters[stegosaurus_OPHAT_OSC2_PITCH_RELEASE],10);
		
		voices[0].osc[0].frequency = fParameters[stegosaurus_KICK_OSC2_PITCH] * 128;
		voices[1].osc[0].frequency = fParameters[stegosaurus_SNARE_OSC2_PITCH] * 128;
		voices[2].osc[0].frequency = fParameters[stegosaurus_CLHAT_OSC2_PITCH] * 128;
		voices[3].osc[0].frequency = fParameters[stegosaurus_OPHAT_OSC2_PITCH] * 128;

	
		
		// MAIN AUDIO LOOP

		for (uint32_t fr = 0; fr < frames; fr++)
		{
			float voice_out[4];
			voice_out[0] = 0;
			voice_out[1] = 0;
			voice_out[2] = 0;
			voice_out[3] = 0;
		
			// DO ENVELOPES
			
			for (int v=0; v<4; v++)
			{
				if ( voices[v].active)
				{

					// DO OSCILLATOR 1 ------------------------------------------------------
			
					if (voices[v].osc[0].active && voices[v].adsr_osc1_amp.state != ENV_STATE_DORMANT)
					{

						//--------- ADSR AMP
						// ATTACK

						switch (voices[v].adsr_osc1_amp.state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v].adsr_osc1_amp.level < 1) 
							{
								voices[v].adsr_osc1_amp.level += voices[v].adsr_osc1_amp.attack;
								if (voices[v].adsr_osc1_amp.level >= 1)
								{
									voices[v].adsr_osc1_amp.level = 1;
									voices[v].adsr_osc1_amp.state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v].adsr_osc1_amp.level > voices[v].adsr_osc1_amp.sustain) 
							{
								voices[v].adsr_osc1_amp.level -=voices[v].adsr_osc1_amp.decay;
									if (voices[v].adsr_osc1_amp.level <= voices[v].adsr_osc1_amp.sustain)
									{
										voices[v].adsr_osc1_amp.level = voices[v].adsr_osc1_amp.sustain;
										voices[v].adsr_osc1_amp.state = ENV_STATE_WAIT;
									}
							}

							if (voices[v].adsr_osc1_amp.level < voices[v].adsr_osc1_amp.sustain ) 
							{
								voices[v].adsr_osc1_amp.level +=voices[v].adsr_osc1_amp.decay;
									if (voices[v].adsr_osc1_amp.level >= voices[v].adsr_osc1_amp.sustain )
									{
										voices[v].adsr_osc1_amp.level = voices[v].adsr_osc1_amp.sustain;
										voices[v].adsr_osc1_amp.state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v].adsr_osc1_amp.level > 0) 
								{
									voices[v].adsr_osc1_amp.level -= voices[v].adsr_osc1_amp.release;
									if (voices[v].adsr_osc1_amp.level <= 0)
									{
										voices[v].adsr_osc1_amp.level = 0;
										voices[v].adsr_osc1_amp.state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}

			

					float env_adsr_osc1_amp_level_db = (voices[v].adsr_osc1_amp.level * voices[v].adsr_osc1_amp.level
						* voices[v].adsr_osc1_amp.level) * fParameters[stegosaurus_VOLUME]; 

						//--------- ADSR pitch
						// ATTACK

						switch (voices[v].adsr_osc1_pitch.state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v].adsr_osc1_pitch.level < 1) 
							{
								voices[v].adsr_osc1_pitch.level += voices[v].adsr_osc1_pitch.attack;
								if (voices[v].adsr_osc1_pitch.level >= 1)
								{
									voices[v].adsr_osc1_pitch.level = 1;
									voices[v].adsr_osc1_pitch.state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v].adsr_osc1_pitch.level > voices[v].adsr_osc1_pitch.sustain) 
							{
								voices[v].adsr_osc1_pitch.level -= voices[v].adsr_osc1_pitch.decay;
									if (voices[v].adsr_osc1_pitch.level <= voices[v].adsr_osc1_pitch.sustain )
									{
										voices[v].adsr_osc1_pitch.level = voices[v].adsr_osc1_pitch.sustain;
										voices[v].adsr_osc1_pitch.state = ENV_STATE_WAIT;
									}
							}

							if (voices[v].adsr_osc1_pitch.level < voices[v].adsr_osc1_pitch.sustain) 
							{
								voices[v].adsr_osc1_pitch.level += voices[v].adsr_osc1_pitch.decay;
									if (voices[v].adsr_osc1_pitch.level >= voices[v].adsr_osc1_pitch.sustain )
									{
										voices[v].adsr_osc1_pitch.level = voices[v].adsr_osc1_pitch.sustain;
										voices[v].adsr_osc1_pitch.state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v].adsr_osc1_pitch.level > 0) 
								{
									voices[v].adsr_osc1_pitch.level -= voices[v].adsr_osc1_pitch.release;
									if (voices[v].adsr_osc1_pitch.level <= 0)
									{
										voices[v].adsr_osc1_pitch.level = 0;
										voices[v].adsr_osc1_pitch.state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}
						
						float adsr_osc1_pitch_level_db = voices[v].adsr_osc1_pitch.level * osc1_pitch[v]; 
						
						voices[v].osc[0].wave_mix = 0;	
						float new_pitch = adsr_osc1_pitch_level_db;
						float pitch_mod = noise1.tick() *  osc1_noise[v];
						new_pitch += pitch_mod;
						new_pitch *= 512;	
						if (new_pitch < 0) new_pitch = 0;
						voices[v].osc[0].frequency = new_pitch;

						voice_out[v] += (osc1_volume[v]  * (voices[v].osc[0].tick() ) * 0.5) * env_adsr_osc1_amp_level_db;
						
					}
						
						
						// DO OSCILLATOR 2------------------------------------------------------
				
						if (voices[v].osc[1].active && voices[v].adsr_osc2_amp.state != ENV_STATE_DORMANT)
						{

							//--------- ADSR AMP
							// ATTACK

							switch (voices[v].adsr_osc2_amp.state)
							{


							case ENV_STATE_ATTACK:
								if (voices[v].adsr_osc2_amp.level < 1) 
								{
									voices[v].adsr_osc2_amp.level += voices[v].adsr_osc2_amp.attack;
									if (voices[v].adsr_osc2_amp.level >= 1)
									{
										voices[v].adsr_osc2_amp.level = 1;
										voices[v].adsr_osc2_amp.state = ENV_STATE_DECAY;
									}
								}
								break;

							// DECAY / SUSTAIN

							case ENV_STATE_DECAY:
							{	
								if (voices[v].adsr_osc2_amp.level > voices[v].adsr_osc2_amp.sustain) 
								{
									voices[v].adsr_osc2_amp.level -=voices[v].adsr_osc2_amp.decay;
										if (voices[v].adsr_osc2_amp.level <= voices[v].adsr_osc2_amp.sustain)
										{
											voices[v].adsr_osc2_amp.level = voices[v].adsr_osc2_amp.sustain;
											voices[v].adsr_osc2_amp.state = ENV_STATE_WAIT;
										}
								}

								if (voices[v].adsr_osc2_amp.level < voices[v].adsr_osc2_amp.sustain ) 
								{
									voices[v].adsr_osc2_amp.level +=voices[v].adsr_osc2_amp.decay;
										if (voices[v].adsr_osc2_amp.level >= voices[v].adsr_osc2_amp.sustain )
										{
											voices[v].adsr_osc2_amp.level = voices[v].adsr_osc2_amp.sustain;
											voices[v].adsr_osc2_amp.state = ENV_STATE_WAIT;
										}
								}
								break;

								// RELEASE

								case ENV_STATE_RELEASE:
									if (voices[v].adsr_osc2_amp.level > 0) 
									{
										voices[v].adsr_osc2_amp.level -= voices[v].adsr_osc2_amp.release;
										if (voices[v].adsr_osc2_amp.level <= 0)
										{
											voices[v].adsr_osc2_amp.level = 0;
											voices[v].adsr_osc2_amp.state = ENV_STATE_DORMANT;
										}
									}
									break;
								}
							}

						float env_adsr_osc2_amp_level_db = (voices[v].adsr_osc2_amp.level * voices[v].adsr_osc2_amp.level
							* voices[v].adsr_osc2_amp.level) * fParameters[stegosaurus_VOLUME]; 

							//--------- ADSR pitch
							// ATTACK

							switch (voices[v].adsr_osc2_pitch.state)
							{


							case ENV_STATE_ATTACK:
								if (voices[v].adsr_osc2_pitch.level < 1) 
								{
									voices[v].adsr_osc2_pitch.level += voices[v].adsr_osc2_pitch.attack;
									if (voices[v].adsr_osc2_pitch.level >= 1)
									{
										voices[v].adsr_osc2_pitch.level = 1;
										voices[v].adsr_osc2_pitch.state = ENV_STATE_DECAY;
									}
								}
								break;

							// DECAY / SUSTAIN

							case ENV_STATE_DECAY:
							{	
								if (voices[v].adsr_osc2_pitch.level > voices[v].adsr_osc2_pitch.sustain) 
								{
									voices[v].adsr_osc2_pitch.level -= voices[v].adsr_osc2_pitch.decay;
										if (voices[v].adsr_osc2_pitch.level <= voices[v].adsr_osc2_pitch.sustain )
										{
											voices[v].adsr_osc2_pitch.level = voices[v].adsr_osc2_pitch.sustain;
											voices[v].adsr_osc2_pitch.state = ENV_STATE_WAIT;
										}
								}

								if (voices[v].adsr_osc2_pitch.level < voices[v].adsr_osc2_pitch.sustain) 
								{
									voices[v].adsr_osc2_pitch.level += voices[v].adsr_osc2_pitch.decay;
										if (voices[v].adsr_osc2_pitch.level >= voices[v].adsr_osc2_pitch.sustain )
										{
											voices[v].adsr_osc2_pitch.level = voices[v].adsr_osc2_pitch.sustain;
											voices[v].adsr_osc2_pitch.state = ENV_STATE_WAIT;
										}
								}
								break;

								// RELEASE

								case ENV_STATE_RELEASE:
									if (voices[v].adsr_osc2_pitch.level > 0) 
									{
										voices[v].adsr_osc2_pitch.level -= voices[v].adsr_osc2_pitch.release;
										if (voices[v].adsr_osc2_pitch.level <= 0)
										{
											voices[v].adsr_osc2_pitch.level = 0;
											voices[v].adsr_osc2_pitch.state = ENV_STATE_DORMANT;
										}
									}
									break;
							}
						}
						
						float adsr_osc2_pitch_level_db = voices[v].adsr_osc2_pitch.level * osc2_pitch[v]; 
						
						voices[v].osc[1].wave_mix = 0;	
						float new_pitch = adsr_osc2_pitch_level_db;
						float pitch_mod = noise1.tick() *  osc2_noise[v];
						new_pitch += pitch_mod;
						new_pitch *= 512;	
						if (new_pitch < 0) new_pitch = 0;
						voices[v].osc[1].frequency = new_pitch;

						voice_out[v] += (osc2_volume[v]  * (voices[v].osc[1].tick() ) * 0.5) * env_adsr_osc2_amp_level_db;
					}
				}
			}
			
			out_left1[fr] += voice_out[0];
			out_right1[fr] += voice_out[0];
			
			out_left1[fr] += voice_out[1];
			out_right1[fr] += voice_out[1];
			
			out_left1[fr] += voice_out[2];
			out_right1[fr] += voice_out[2];
			
			out_left1[fr] += voice_out[3];
			out_right1[fr] += voice_out[3];
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

			
			if (ftell(fp) == 17720 )
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
	
	for (int o=0; o<number_of_oscillators; o++)
	{
		for (int v=0; v<number_of_voices; v++)
		{
			if (new_stegosaurus->wavetables.size() > 0)
			{
				new_stegosaurus->voices[v].osc[o].wave_a = new_stegosaurus->wavetables[0].buffer;
				new_stegosaurus->voices[v].osc[o].wave_b = new_stegosaurus->wavetables[0].buffer;
				new_stegosaurus->voices[v].osc[o].length = length;
			}
			else
			{
				new_stegosaurus->voices[v].osc[o].wave_a = NULL;
				new_stegosaurus->voices[v].osc[o].wave_b = NULL;
				new_stegosaurus->voices[v].osc[o].length = 0;
			}
		}
	}

	return new_stegosaurus;
}

END_NAMESPACE_DISTRHO











