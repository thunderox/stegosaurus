

#include "oscillator.hpp"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "fastmaths.hpp"

#define ENV_STATE_ATTACK 1
#define ENV_STATE_DECAY 2
#define ENV_STATE_SUSTAIN 3
#define ENV_STATE_RELEASE 4
#define ENV_STATE_WAIT 5
#define ENV_STATE_DORMANT 6
#define ENV_STATE_KILL 7

const int number_of_oscillators = 4;

struct wavetable
{
	string name;
	float* buffer;
	long length;
};

struct adsr
{
	float attack;
	float decay;
	float sustain;
	float release;
	float level;
	int state;
};

class voice
{

	public:

	voice();
	~voice();
	void init(double);

	oscillator osc[number_of_oscillators];
	
	double sample_rate;

	bool active;

	float volume;
	int out_dest;
	float pan;
	float frequency;

	float buf0_left,buf1_left;
	float f_left, pc_left, q_left;             //filter coefficients
	float bf0_left, bf1_left, bf2_left, bf3_left, bf4_left;  //filter buffers (beware denormals!)
	float t1_left, t2_left;              //temporary buffers
	float selectivity_left, gain1_left, gain2_left, ratio_left, cap_left; 

	float filter_frequency_left,filter_resonance_left;
	
	float buf0_right,buf1_right;
	float f_right, pc_right, q_right;             //filter coefficients
	float bf0_right, bf1_right, bf2_right, bf3_right, bf4_right;  //filter buffers (beware denormals!)
	float t1_right, t2_right;              //temporary buffers
	float selectivity_right, gain1_right, gain2_right, ratio_right, cap_right; 

	float filter_frequency_right,filter_resonance_right;

	// Envelopes
		
	adsr adsr_osc1_amp;
	adsr adsr_osc1_pitch;
	adsr adsr_osc2_amp;
	adsr adsr_osc2_pitch;

};
	
	
	

	
	
	
