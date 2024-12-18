
#include "nix-echo.hpp"

using namespace std;

//------------------------------------------------------------------------------------
// INITIALISE
nixecho::nixecho()
{
	float newFloat = 0;
	buffer_left.resize ( 1024*64, newFloat );
	buffer_right.resize ( 1024*64, newFloat );

	old_in_left = 0;
	old_in_right = 0;

	for (unsigned long int x=0; x<buffer_left.size(); x++)
	{
		buffer_left[x] = 0;
		buffer_right[x] = 0;
	}

	play_head = 0;
	delay_left = 0;
	stereo_width = 64;
	delay_right = stereo_width;
	feedback_left = 0;
	feedback_right = 0;
	echo_decay = 0.8;
	echo_speed = 19329;
	
	eq = new EQSTATE();
	eq2 = new EQSTATE();
	
        set_sample_rate(44100);	
}

//------------------------------------------------------------------------------------
// DECONSTRUCTOR
nixecho::~nixecho()
{
}

//------------------------------------------------------------------------------------
// EMPTY BUFFER
void nixecho::clear_buffer()
{
	for (unsigned long int x=0; x<buffer_left.size(); x++)
	{
		buffer_left[x] = 0;
		buffer_right[x] = 0;
	}
}

//------------------------------------------------------------------------------------

void nixecho::set_sample_rate(float s_rate)
{
	init_3band_state(eq,880,5000,s_rate);		
	init_3band_state(eq2,880,5000,s_rate);	
}

//------------------------------------------------------------------------------------
// CLEAR BUFFER AND RESET ECHO
void nixecho::reset()
{


	for (unsigned long int x=0; x<buffer_left.size(); x++)
	{
		buffer_left[x] = 0;
		buffer_right[x] = 0;
	}
}

//------------------------------------------------------------------------------------
// DO ECHO STUFF - left

float nixecho::do_left(float in)
{
	if (!std::isnan(feedback_left) && feedback_left > -1 && feedback_left < 1)
	{
		buffer_left[play_head] = feedback_left;
	}


 	play_head++;
	if (play_head >= echo_speed) {play_head = 0; }

	delay_left++;
	if (delay_left >= echo_speed) {delay_left = 0; }

	if (bounce)
		feedback_left = in + do_3band(eq, buffer_right[delay_left] * echo_decay);
	else
		feedback_left = in + do_3band(eq, buffer_left[delay_left] * echo_decay);
		
	return feedback_left;

}

//------------------------------------------------------------------------------------
// DO ECHO STUFF - right

float nixecho::do_right(float in)
{
	if (!std::isnan(feedback_right) && feedback_right > -1 && feedback_right  < 1)
	{
		buffer_right[play_head] = feedback_right;
	}
	
	delay_right++;
	if (delay_right >= echo_speed) {delay_right = 0; }
	
	if (bounce)
		feedback_right = in + do_3band(eq2, buffer_left[delay_right] * echo_decay);
	else
		feedback_right = in + do_3band(eq2, buffer_right[delay_right] * echo_decay);	
	
	return feedback_right;

}







