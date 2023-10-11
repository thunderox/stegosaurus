
#include "oscillator.hpp"



//---------------------------------------------------------------------------------------------------

oscillator::oscillator()
{
	start_phase = true;
	frequency = 1;
	index = 0;
	increment = 1;
	bandlimit_offset = 0;
	active = false;
	noise = 0;
	length = 17720;
	
	sample_rate = 44100;
	
    sine_generator sg;
}

//---------------------------------------------------------------------------------------------------

oscillator::~oscillator()
{

}


//-----------------------------------------------------------
// Play wavetable 

float oscillator::tick()
{
	if (frequency < 0.01) frequency = 0.01;
	/*
	tuning = 1;
	float frq = frequency * tuning;
	int index_int = index;
	

	
	float result1 = 0, result2 = 0;

	if (index == index_int) 
	{
		result1 = wave_a[bandlimit_offset+index_int];
		result2 = wave_b[bandlimit_offset+index_int];		
	}
	else
	{
		float fraction1 = index - index_int;
		float fraction2 = 1 - fraction1;

		if (index < length-1)
		{	

			result1 =  wave_a[index_int+bandlimit_offset] * fraction2;
			result1 += wave_a[index_int+1+bandlimit_offset] * fraction1;
			result2 =  wave_b[index_int+bandlimit_offset] * fraction2;
			result2 += wave_b[index_int+1+bandlimit_offset] * fraction1;	
		}
		else 
		{
			result1 =  wave_a[index_int+bandlimit_offset] * fraction2;
			result1 += wave_a[bandlimit_offset] * fraction1;
			result2 =  wave_b[index_int+bandlimit_offset] * fraction2;
			result2 += wave_b[bandlimit_offset] * fraction1;
		}
	}	

	index += increment;
	
	

	if (index > length - 1 )
	{	
		index -= length;	
		start_phase = true;

		int bandlimit_number = (note / 10);
		if (bandlimit_number<0) bandlimit_number = 0;
		if (bandlimit_number>11) bandlimit_number = 11;
		bandlimit_offset = (bandlimit_number*length);
	}
	
	*/

	// increment = (44100 * frequency) / sample_rate;
	// cout << result1 << " - " << result2 << " - " << frequency << endl;
	
	sg.set_frequency(frequency * 2 * 3.141592653589793 / 8000);
	double test[16];
	sg.generate(1,&test[0]);

	return test[0];
	// return (result2 * wave_mix) + (result1 * (1-wave_mix));

}





	
	
	
