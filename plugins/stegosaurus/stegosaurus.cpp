
#include "DistrhoPlugin.hpp"
#include "voice.h"

using namespace std;

START_NAMESPACE_DISTRHO

const int max_voices = 4;

class stegosaurus : public Plugin
{
	public:
	
	float fParameters[kParameterCount];
	double srate;
	int keys[128];
	voice voices[max_voices];

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

	private:

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(stegosaurus);
};


Plugin *createPlugin() { return new stegosaurus(); }

END_NAMESPACE_DISTRHO

