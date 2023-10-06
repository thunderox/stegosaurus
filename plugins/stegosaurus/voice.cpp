
#include "voice.h"

//---------------------------------------------------------------------------------------------------------------------------

voice::voice()
{
}

//---------------------------------------------------------------------------------------------------------------------------

voice::~voice()
{
}

//---------------------------------------------------------------------------------------------------------------------------

void voice::init(double _sample_rate)
{
	sample_rate = _sample_rate;
	
	for (int x=0; x<number_of_oscillators; x++)
	{
		osc[x].sample_rate = _sample_rate;
	}
	
	active = false;
	volume = 0;
	out_dest = 0;
	pan = 0.5;
	frequency = 50;
	
	// Init filter left

	buf0_left=0; buf1_left=0;

  	bf0_left=0; bf1_left=0; bf2_left=0; bf3_left=0; bf4_left=0;  //filter buffers (beware denormals!)
  	t1_left=0; t2_left=0;              //temporary buffers

	selectivity_left=90, gain1_left=0, gain2_left=0.25, ratio_left=7, cap_left=0; 

	// Init filter right

	buf0_right=0; buf1_right=0;

	f_right=0; pc_right=0; q_right=0;             //filter coefficients
  	bf0_right=0; bf1_right=0; bf2_right=0; bf3_right=0; bf4_right=0;  //filter buffers (beware denormals!)
  	t1_right=0; t2_right=0;              //temporary buffers

	selectivity_right=90, gain1_right=0, gain2_right=0.25, ratio_right=7, cap_right=0; 


	osc1_amp.state = ENV_STATE_DORMANT;
	osc1_amp.level = 0;
	osc1_pitch.state = ENV_STATE_DORMANT;
	osc1_pitch.level = 0;

	osc2_amp.state = ENV_STATE_DORMANT;
	osc2_amp.level = 0;
	osc2_pitch.state = ENV_STATE_DORMANT;
	osc2_pitch.level = 0;
}




