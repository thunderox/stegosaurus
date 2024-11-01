
#include "DistrhoPlugin.hpp"
#include "voice.h"
#include "nix-echo.hpp"
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

const int number_of_voices = 6;

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
	noise noise1;
	nixecho* echo;

	stegosaurus() : Plugin(kParameterCount, 0, 0)
	{
		std::memset(fParameters, 0, sizeof(float)*kParameterCount);
		srate = getSampleRate();
		for (int x=0; x<128; x++) { keys[x] = -1; }
		fParameters[kParameterBufferSize] = getBufferSize();
		
		for (int v=0; v<number_of_voices; v++)
		{
			voices[v].init(srate);
		}
		
			echo = new nixecho();
			echo->set_sample_rate(srate);
			echo->echo_speed = 3800;
			echo->echo_decay = 0.5;
			
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
		return d_version(0, 0, 1);
	}

	int64_t getUniqueId() const
	{
		return d_cconst('T','O','S','S');
	}
	
	void set_param (Parameter& param, const char* name, int index, float min, float max, float def)
	{	
		param.name = name;
		param.symbol = name;
		param.ranges.min = min;
		param.ranges.max = max;
		param.ranges.def = def;
		fParameters[index] = def; 
		
	}	
		
 	void initParameter (uint32_t index, Parameter& parameter) override
 	{
 	
		switch (index)
		{
		
			// MASTER CONTROLS -----------------------------
											
			case stegosaurus_VOLUME:
				set_param (parameter, "stegosaurus_VOLUME", index, 0.0f, 2.0f, 1.8f);
				break;
				
			// KICK DRUM ------------------------------------
			
			// OSCILLATOR ONE
						
			case stegosaurus_KICK_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_KICK_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC1_NOISE:
				set_param (parameter, "stegosaurus_KICK_OSC1_NOISE", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_KICK_OSC1_PITCH:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH", index, 0.0f, 0.25f, 0.005f);
				break;
			case stegosaurus_KICK_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_KICK_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_KICK_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.3565f);
				break;
			case stegosaurus_KICK_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_KICK_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_KICK_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.393f);
				break;
			case stegosaurus_KICK_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.40300003f);
				break;
			case stegosaurus_KICK_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH_RELEASE", index, 0.0f, 1.0f, 0.39129999f);
				break;
			case stegosaurus_KICK_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_KICK_OSC1_PITCH_ADSR2", index, 0.0f, 0.25f, 0.00799954f);
				break;
			case stegosaurus_KICK_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_KICK_OSC1_VOLUME", index, 0.0f, 1.0f, 0.95370001f);
				break;
				
			// OSCILLATOR TWO
						
			case stegosaurus_KICK_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_KICK_OSC2_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC2_NOISE:
				set_param (parameter, "stegosaurus_KICK_OSC2_NOISE", index, 0.0f, 1.0f, 0.47222221f);
				break;
			case stegosaurus_KICK_OSC2_PITCH:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_KICK_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_KICK_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_KICK_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 0.5911f);
				break;
			case stegosaurus_KICK_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_KICK_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_KICK_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 0.33700001f);
				break;
			case stegosaurus_KICK_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 0.56547797f);
				break;
			case stegosaurus_KICK_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_KICK_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 0.27169999f);
				break;
			case stegosaurus_KICK_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_KICK_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 0.1913327f);
				break;
			case stegosaurus_KICK_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_KICK_OSC2_VOLUME", index, 0.0f, 1.0f, 0.412f);
				break;
				
			case stegosaurus_KICK_OSC3_PITCH:
				set_param (parameter, "stegosaurus_KICK_OSC3_PITCH", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_KICK_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_KICK_OSC3_VOLUME", index, 0.0f, 1.0f, 0.0f);
				break;
				
			// SNARE DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_SNARE_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_SNARE_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC1_NOISE:
				set_param (parameter, "stegosaurus_SNARE_OSC1_NOISE", index, 0.0f, 1.0f, 0.16666663f);
				break;			
			case stegosaurus_SNARE_OSC1_PITCH:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH", index, 0.0f, 2.0f, 1.9074f);
				break;	
			case stegosaurus_SNARE_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_SNARE_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_SNARE_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.3578f);
				break;	
			case stegosaurus_SNARE_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_SNARE_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_SNARE_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.3596);
				break;
			case stegosaurus_SNARE_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_SNARE_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_SNARE_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH_RELEASE", index, 0.0f, 2.0f, 0.1087f);
				break;
			case stegosaurus_SNARE_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_SNARE_OSC1_PITCH_ADSR2", index, 0.0f, 2.0f, 0.25189999f);
				break;
			case stegosaurus_SNARE_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_SNARE_OSC1_VOLUME", index, 0.0f, 1.0f, 0.2848f);
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_SNARE_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_SNARE_OSC2_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC2_NOISE:
				set_param (parameter, "stegosaurus_SNARE_OSC2_NOISE", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_SNARE_OSC2_PITCH:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH", index, 0.0f, 2.0f, 0.00879884f);
				break;
			case stegosaurus_SNARE_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_SNARE_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_SNARE_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 0.39400008f);
				break;
			case stegosaurus_SNARE_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_SNARE_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_SNARE_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 0.38870001);
				break;
			case stegosaurus_SNARE_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 0.40699992f);
				break;
			case stegosaurus_SNARE_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_SNARE_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 0.39021999f);
				break;
			case stegosaurus_SNARE_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_SNARE_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 0.02700126);
				break;
			case stegosaurus_SNARE_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_SNARE_OSC2_VOLUME", index, 0.0f, 1.0f, 0.63870001f);
				break;
				
			case stegosaurus_SNARE_OSC3_PITCH:
				set_param (parameter, "stegosaurus_SNARE_OSC3_PITCH", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_SNARE_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_SNARE_OSC3_VOLUME", index, 0.0f, 1.0f, 0.0f);
				break;
				
			// CLOSED HAT DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_CLHAT_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC1_NOISE:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_NOISE", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH", index, 0.0f, 2.0f, 1.5370f);
				break;
			case stegosaurus_CLHAT_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.4465f);
				break;
			case stegosaurus_CLHAT_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.98f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH_RELEASE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_PITCH_ADSR2", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_CLHAT_OSC1_VOLUME", index, 0.0f, 1.0f, 0.3f);
				break;

			// OSCILLATOR TWO
				
			case stegosaurus_CLHAT_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_ACTIVE", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC2_NOISE:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_NOISE", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLHAT_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_CLHAT_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_CLHAT_OSC2_VOLUME", index, 0.0f, 1.0f, 0.0f);
				break;
				
			case stegosaurus_CLHAT_OSC3_PITCH:
				set_param (parameter, "stegosaurus_CLHAT_OSC3_PITCH", index, 0.0f, 1.0f, 0.1185f);
				break;
			case stegosaurus_CLHAT_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_CLHAT_OSC3_VOLUME", index, 0.0f, 1.0f, 0.9259f);
				break;


			// OPEN HAT DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_OPHAT_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC1_NOISE:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_NOISE", index, 0.0f, 1.0f, 0.03481483f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH", index, 0.0f, 2.0f, 1.5925926f);
				break;
			case stegosaurus_OPHAT_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.35999981f);
				break;
			case stegosaurus_OPHAT_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.36500013f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH_RELEASE", index, 0.0f, 1.0f, 0.1522f);
				break;
			case stegosaurus_OPHAT_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_PITCH_ADSR2", index, 0.0f, 2.0f, 0.03703701f);
				break;
			case stegosaurus_OPHAT_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_OPHAT_OSC1_VOLUME", index, 0.0f, 1.0f, 0.16370004f);
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_OPHAT_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_NOISE:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_NOISE", index, 0.0f, 1.0f, 0.14814812f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH", index, 0.0f, 2.0f, 1.4074074f);
				break;
			case stegosaurus_OPHAT_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 0.5086999f);
				break;
			case stegosaurus_OPHAT_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_OPHAT_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 0.48148143f);
				break;
			case stegosaurus_OPHAT_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_OPHAT_OSC2_VOLUME", index, 0.0f, 1.0f, 1.0f);
				break;
				
			case stegosaurus_OPHAT_OSC3_PITCH:
				set_param (parameter, "stegosaurus_OPHAT_OSC3_PITCH", index, 0.0f, 1.0f, 0.33333337f);
				break;
			case stegosaurus_OPHAT_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_OPHAT_OSC3_VOLUME", index, 0.0f, 1.0f, 0.87037039f);
				break;
				
				
				
			// CLAP DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_CLAP_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_CLAP_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC1_NOISE:
				set_param (parameter, "stegosaurus_CLAP_OSC1_NOISE", index, 0.0f, 1.0f, 0.1f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH", index, 0.0f, 2.0f, 0.0f);
				break;		
			case stegosaurus_CLAP_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_CLAP_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 0.8126f);
				break;
			case stegosaurus_CLAP_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_CLAP_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.35779998f);
				break;
			case stegosaurus_CLAP_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_CLAP_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_CLAP_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.35959998f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH_RELEASE", index, 0.0f, 1.0f, 0.18220001f);
				break;
			case stegosaurus_CLAP_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_CLAP_OSC1_PITCH_ADSR2", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_CLAP_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_CLAP_OSC1_VOLUME", index, 0.0f, 2.0f, 0.25f);
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_CLAP_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_CLAP_OSC2_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC2_NOISE:
				set_param (parameter, "stegosaurus_CLAP_OSC2_NOISE", index, 0.0f, 1.0f, 0.12037039f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH", index, 0.0f, 2.0f, 1.13888884f);
				break;
			case stegosaurus_CLAP_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_CLAP_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_CLAP_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 0.59869987f);
				break;
			case stegosaurus_CLAP_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_CLAP_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_CLAP_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 0.36652166f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 0.96000004f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 0.52956504f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 0.16304348f);
				break;
			case stegosaurus_CLAP_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_CLAP_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 1.13888884f);
				break;
			case stegosaurus_CLAP_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_CLAP_OSC2_VOLUME", index, 0.0f, 1.0f, 0.4675926f);
				break;
				
			case stegosaurus_CLAP_OSC3_PITCH:
				set_param (parameter, "stegosaurus_CLAP_OSC3_PITCH", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_CLAP_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_CLAP_OSC3_VOLUME", index, 0.0f, 1.0f, 0.0f);
				break;
				
				
				
			// COWBELL DRUM ------------------------------------
			
			// OSCILLATOR ONE
				
			case stegosaurus_COWBELL_OSC1_ACTIVE:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC1_NOISE:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_NOISE", index, 0.0f, 1.0f, 0.1f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH", index, 0.0f, 2.0f, 0.0f);
				break;		
			case stegosaurus_COWBELL_OSC1_AMP_ATTACK:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_AMP_ATTACK", index, 0.0f, 1.0f, 0.8126f);
				break;
			case stegosaurus_COWBELL_OSC1_AMP_DECAY:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_AMP_DECAY", index, 0.0f, 1.0f, 0.35779998f);
				break;
			case stegosaurus_COWBELL_OSC1_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC1_AMP_RELEASE:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_AMP_RELEASE", index, 0.0f, 1.0f, 0.35959998f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH_DECAY:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH_DECAY", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH_RELEASE", index, 0.0f, 1.0f, 0.18220001f);
				break;
			case stegosaurus_COWBELL_OSC1_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_PITCH_ADSR2", index, 0.0f, 2.0f, 0.0f);
				break;
			case stegosaurus_COWBELL_OSC1_VOLUME:
				set_param (parameter, "stegosaurus_COWBELL_OSC1_VOLUME", index, 0.0f, 2.0f, 0.28481483f);
				break;
				
			// OSCILLATOR TWO
				
			case stegosaurus_COWBELL_OSC2_ACTIVE:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_ACTIVE", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC2_NOISE:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_NOISE", index, 0.0f, 1.0f, 0.12037039f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH", index, 0.0f, 2.0f, 1.13888884f);
				break;
			case stegosaurus_COWBELL_OSC2_AMP_ATTACK:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_AMP_ATTACK", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC2_AMP_DECAY:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_AMP_DECAY", index, 0.0f, 1.0f, 0.59869987f);
				break;
			case stegosaurus_COWBELL_OSC2_AMP_SUSTAIN:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_AMP_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC2_AMP_RELEASE:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_AMP_RELEASE", index, 0.0f, 1.0f, 0.36652166f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH_ATTACK:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH_ATTACK", index, 0.0f, 1.0f, 0.96000004f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH_DECAY:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH_DECAY", index, 0.0f, 1.0f, 0.52956504f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH_SUSTAIN:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH_SUSTAIN", index, 0.0f, 1.0f, 1.0f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH_RELEASE:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH_RELEASE", index, 0.0f, 1.0f, 0.16304348f);
				break;
			case stegosaurus_COWBELL_OSC2_PITCH_ADSR2:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_PITCH_ADSR2", index, 0.0f, 2.0f, 1.13888884f);
				break;
			case stegosaurus_COWBELL_OSC2_VOLUME:
				set_param (parameter, "stegosaurus_COWBELL_OSC2_VOLUME", index, 0.0f, 1.0f, 0.4675926f);
				break;
				
			case stegosaurus_COWBELL_OSC3_PITCH:
				set_param (parameter, "stegosaurus_COWBELL_OSC3_PITCH", index, 0.0f, 1.0f, 0.0f);
				break;
			case stegosaurus_COWBELL_OSC3_VOLUME:
				set_param (parameter, "stegosaurus_COWBELL_OSC3_VOLUME", index, 0.0f, 1.0f, 0.0f);
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
		(void) index;
		(void) program_name;
	}

    	//----------------------------------------------	//----------------------------------------------
    	
	void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
	{
	
		(void) inputs;
		
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
				if (midi_note == 60 || midi_note == 35 ) // C3 - KICK
				{
					// cout << "Kick triggered" << endl;
					
					voices[0].active = true;
					voices[0].reset_phase();
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
		
				if (midi_note == 62 || midi_note == 38) // D3 - SNARE
				{
					// cout << "Snare triggered" << endl;
					
					voices[1].active = true;
					voices[1].reset_phase();
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
				
				if (midi_note == 64 || midi_note == 42) // E3 - CLOSED HAT
				{
					// cout << "Closed Hat triggered" << endl;
					
					voices[2].active = true;
					voices[2].reset_phase();
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
				
				if (midi_note == 65 || midi_note == 46) // F3 - OPEN HAT
				{
					// cout << "Open Hat triggered" << endl;
					
					voices[3].active = true;
					voices[3].reset_phase();
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
				
				if (midi_note == 67 || midi_note == 39) // G3 - CLAP
				{
					// cout << "Claptriggered" << endl;
					
					voices[4].active = true;
					voices[4].reset_phase();
					voices[4].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[4].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[4].adsr_osc1_amp.level = 0;
					voices[4].adsr_osc1_pitch.level = 0;
					voices[4].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[4].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[4].adsr_osc2_amp.level = 0;
					voices[4].adsr_osc2_pitch.level = 0;
					voices[4].frequency = midi_note;
					voices[4].volume = (float)midi_volume/128 ;
					keys[midi_note] = 0;
				}
				
				if (midi_note == 69 || midi_note == 56) // A3 - COWBELL
				{
					// cout << "Cowbell triggered" << endl;
					
					voices[5].active = true;
					voices[5].reset_phase();
					voices[5].adsr_osc1_amp.state = ENV_STATE_ATTACK;
					voices[5].adsr_osc1_pitch.state = ENV_STATE_ATTACK;
					voices[5].adsr_osc1_amp.level = 0;
					voices[5].adsr_osc1_pitch.level = 0;
					voices[5].adsr_osc2_amp.state = ENV_STATE_ATTACK;
					voices[5].adsr_osc2_pitch.state = ENV_STATE_ATTACK;
					voices[5].adsr_osc2_amp.level = 0;
					voices[5].adsr_osc2_pitch.level = 0;
					voices[5].frequency = midi_note;
					voices[5].volume = (float)midi_volume/128 ;
					keys[midi_note] = 0;
				}
			}
			
			// CHECK FOR NOTE OFF - OMNI MODE
			
		      	if ( (midi_command >= 0x80 && midi_command <= 0x8f)
				||  (midi_command >= 0x90 && midi_command <= 0x9f && midi_volume == 0) )
			{
				if (midi_note == 60 || midi_note == 35 ) // C3 - KICK
				{
					voices[0].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[0].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 62 || midi_note == 38) // D3 - SNARE
				{
					voices[1].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[1].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 64 || midi_note == 42) // E3 - CLOSED HAT
				{
					voices[2].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[2].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 65 || midi_note == 46) // F3 - OPEN HAT
				{
					voices[3].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[3].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 67 || midi_note == 39) // G3 - CLAP
				{
					voices[4].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[4].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[4].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[4].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
				}
				
				if (midi_note == 69 || midi_note == 56) // G3 - COWBELL
				{
				
					voices[5].adsr_osc1_amp.state = ENV_STATE_RELEASE;
					voices[5].adsr_osc1_pitch.state = ENV_STATE_RELEASE;
					voices[5].adsr_osc2_amp.state = ENV_STATE_RELEASE;
					voices[5].adsr_osc2_pitch.state = ENV_STATE_RELEASE;
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
		voices[4].osc[0].active = fParameters[stegosaurus_CLAP_OSC1_ACTIVE];
		voices[4].osc[1].active = fParameters[stegosaurus_CLAP_OSC2_ACTIVE];
		voices[5].osc[0].active = fParameters[stegosaurus_COWBELL_OSC1_ACTIVE];
		voices[5].osc[1].active = fParameters[stegosaurus_COWBELL_OSC2_ACTIVE];
		
		float osc1_noise[number_of_voices];
		float osc1_pitch[number_of_voices];
		float osc1_pitch_adsr2[number_of_voices];
		float osc1_volume[number_of_voices];
		
		float osc2_volume[number_of_voices];
		float osc2_pitch[number_of_voices];
		float osc2_pitch_adsr2[number_of_voices];
		float osc2_noise[number_of_voices];
		
		float osc3_pitch[number_of_voices];
		float osc3_volume[number_of_voices];

		for (int v=0; v<number_of_voices; v++)
		{
			osc1_noise[v] = fParameters[stegosaurus_KICK_OSC1_NOISE + (v*28) ];
			osc1_pitch[v] = fParameters[stegosaurus_KICK_OSC1_PITCH + (v*28) ];
			osc1_pitch_adsr2[v] = fParameters[stegosaurus_KICK_OSC1_PITCH_ADSR2 + (v*28) ];
			osc1_volume[v] = fParameters[stegosaurus_KICK_OSC1_VOLUME + (v*28) ];
			

			osc2_noise[v] = fParameters[stegosaurus_KICK_OSC2_NOISE + (v*28) ];
			osc2_pitch[v] = fParameters[stegosaurus_KICK_OSC2_PITCH + (v*28) ];
			osc2_pitch_adsr2[v] = fParameters[stegosaurus_KICK_OSC2_PITCH_ADSR2 + (v*28) ];
			osc2_volume[v] = fParameters[stegosaurus_KICK_OSC2_VOLUME + (v*28) ];
			
			osc3_pitch[v] = fParameters[stegosaurus_KICK_OSC3_PITCH + (v*28) ];
			osc3_volume[v] = fParameters[stegosaurus_KICK_OSC3_VOLUME + (v*28) ];
			
			voices[v].adsr_osc1_amp.attack = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_ATTACK + (v*28) ],10); 
			voices[v].adsr_osc1_amp.decay = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_DECAY + (v*28) ],10); 
			voices[v].adsr_osc1_amp.sustain = 1-fParameters[stegosaurus_KICK_OSC1_AMP_SUSTAIN + (v*28) ]; 
			voices[v].adsr_osc1_amp.release = fast_pow(fParameters[stegosaurus_KICK_OSC1_AMP_RELEASE + (v*28) ],10); 
			voices[v].adsr_osc1_pitch.attack = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_ATTACK + (v*28) ],10); 
			voices[v].adsr_osc1_pitch.decay = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_DECAY + (v*28) ],10); 
			voices[v].adsr_osc1_pitch.sustain = 1-fParameters[stegosaurus_KICK_OSC1_PITCH_SUSTAIN + (v*28) ]; 
			voices[v].adsr_osc1_pitch.release = fast_pow(fParameters[stegosaurus_KICK_OSC1_PITCH_RELEASE + (v*28) ],10); 
			voices[v].osc[0].frequency = fParameters[stegosaurus_KICK_OSC1_PITCH + (v*28) ] * 128;

			voices[v].adsr_osc2_amp.attack = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_ATTACK + (v*28) ],10); 
			voices[v].adsr_osc2_amp.decay = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_DECAY + (v*28) ],10); 
			voices[v].adsr_osc2_amp.sustain = 1-fParameters[stegosaurus_KICK_OSC2_AMP_SUSTAIN + (v*28) ]; 
			voices[v].adsr_osc2_amp.release = fast_pow(fParameters[stegosaurus_KICK_OSC2_AMP_RELEASE + (v*28) ],10); 
			voices[v].adsr_osc2_pitch.attack = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_ATTACK + (v*28) ],10);
			voices[v].adsr_osc2_pitch.decay = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_DECAY + (v*28) ],10);
			voices[v].adsr_osc2_pitch.sustain = 1-fParameters[stegosaurus_KICK_OSC2_PITCH_SUSTAIN + (v*28) ]; 
			voices[v].adsr_osc2_pitch.release = fast_pow(fParameters[stegosaurus_KICK_OSC2_PITCH_RELEASE + (v*28) ],10);
			voices[v].osc[1].frequency = fParameters[stegosaurus_KICK_OSC2_PITCH + (v*28) ] * 128;
		}
		


		// MAIN AUDIO LOOP

		for (uint32_t fr = 0; fr < frames; fr++)
		{
			float voice_out[number_of_voices];
			
			for (int v=0; v<number_of_voices; v++)
			{
				voice_out[v] = 0;
			}
		
			// DO ENVELOPES
			
			for (int v=0; v<number_of_voices; v++)
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
						
						float adsr_osc1_pitch_level_db = voices[v].adsr_osc1_pitch.level * osc1_pitch_adsr2[v]; 
						
						voices[v].osc[2].frequency = 256 * osc3_pitch[v];
						
						voices[v].osc[0].wave_mix = 0;	
						float new_pitch = adsr_osc1_pitch_level_db;
						float pitch_mod = (8 * noise1.tick() ) *  osc1_noise[v];
						new_pitch += pitch_mod;
						new_pitch += osc1_pitch[v];
						new_pitch += voices[v].osc[2].tick() * osc3_volume[v];
						new_pitch *= 1024;	
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
						
						float adsr_osc2_pitch_level_db = voices[v].adsr_osc2_pitch.level * osc2_pitch_adsr2[v]; 
						
						voices[v].osc[1].wave_mix = 0;	
						float new_pitch = adsr_osc2_pitch_level_db;
						float pitch_mod = (8 * noise1.tick() ) *  osc2_noise[v];
						new_pitch += pitch_mod;
						new_pitch += osc2_pitch[v];
						new_pitch *= 1024;	
						if (new_pitch < 0) new_pitch = 0;
						voices[v].osc[1].frequency = new_pitch;

						voice_out[v] += (osc2_volume[v]  * (voices[v].osc[1].tick() ) * 0.5) * env_adsr_osc2_amp_level_db;
					}
				}
			}
			
			for (int v=0; v<number_of_voices; v++)
			{
				if (v == 4)
				{
					out_left1[fr] += echo->do_left( voice_out[v]);
					out_right1[fr] += echo->do_right( voice_out[v] );
				}
				else
				{
					out_left1[fr] += voice_out[v];
					out_right1[fr] += voice_out[v];
				}
			}
			
		}

	}


	//-------------------------------------------------------------------------------------------------------

	private:

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(stegosaurus);
};


Plugin *createPlugin()
{

	stegosaurus* new_stegosaurus = new stegosaurus();


	return new_stegosaurus;
}

END_NAMESPACE_DISTRHO











