
#include "stegosaurus_synth.h"
#include "noise.h"

voice* voices[4];
int next_voice = 0;
int keys[128];

void stegosaurus_synth_init(int sample_rate)
{

	noise_init();

	wavetable_set_sample_rate(sample_rate);

	for (int x=0; x<128; x++)
	{
		keys[x] = -1;
	}

	for (int v=0; v<4; v++)
	{
		voices[v] = (voice*)malloc(sizeof(voice));
		voices[v]->active = false;
		voices[v]->volume = 0;
		voices[v]->out_dest = 0;
		voices[v]->pan = 0.5;
		voices[v]->frequency = 50;

		for (int y=0; y<3; y++)
		{
			voices[v]->osc[y].wave1_number = 0;	
			voices[v]->osc[y].wave2_number = 0;
			voices[v]->osc[y].wave_mix = 0;
			voices[v]->osc[y].start_phase = true;
			voices[v]->osc[y].frequency = 0;
			voices[v]->osc[y].index = 0;
			voices[v]->osc[y].increment = 1;
			voices[v]->osc[y].bandlimit_offset = 0;
		}
	



		// Init filter left

		voices[v]->buf0_left=0; voices[v]->buf1_left=0;

	  	voices[v]->bf0_left=0; voices[v]->bf1_left=0; voices[v]->bf2_left=0; voices[v]->bf3_left=0; voices[v]->bf4_left=0;  //filter buffers (beware denormals!)
	  	voices[v]->t1_left=0; voices[v]->t2_left=0;              //temporary buffers

		voices[v]->selectivity_left=90, voices[v]->gain1_left=0, voices[v]->gain2_left=0.25, voices[v]->ratio_left=7, voices[v]->cap_left=0; 

		// Init filter right

		voices[v]->buf0_right=0; voices[v]->buf1_right=0;

		voices[v]->f_right=0; voices[v]->pc_right=0; voices[v]->q_right=0;             //filter coefficients
	  	voices[v]->bf0_right=0; voices[v]->bf1_right=0; voices[v]->bf2_right=0; voices[v]->bf3_right=0; voices[v]->bf4_right=0;  //filter buffers (beware denormals!)
	  	voices[v]->t1_right=0; voices[v]->t2_right=0;              //temporary buffers

		voices[v]->selectivity_right=90, voices[v]->gain1_right=0, voices[v]->gain2_right=0.25, voices[v]->ratio_right=7, voices[v]->cap_right=0; 


		voices[v]->env_osc1_amp_state = ENV_STATE_DORMANT;
		voices[v]->env_osc1_amp_level = 0;

		voices[v]->env_wave_state = ENV_STATE_DORMANT;
		voices[v]->env_wave_level = 0;
	}

	wavetable_initialise();

	wavetable_make_wave(0, stegosaurus_self->bundle_path, "sine");
}

void stegosaurus_synth_cleanup()
{
	for (int x=0; x<4; x++)
	{
		free(voices[x]);		
	}

	noise_cleanup();
}

void stegosaurus_synth_play(float* const out_one, float* const out_two, float* const out_three,float* const out_four, uint32_t n_frames)
{

	float master_volume = stegosaurus_self->params[stegosaurus_VOLUME][0]; 

	//-------------------------

	bool osc1_active[4];
	osc1_active[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_ACTIVE][0];
	osc1_active[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_ACTIVE][0];
	osc1_active[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_ACTIVE][0];
	osc1_active[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_ACTIVE][0];

	float osc1_amp_attack[4];
	osc1_amp_attack[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_AMP_ATTACK][0],10); 
	osc1_amp_attack[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_AMP_ATTACK][0],10); 
	osc1_amp_attack[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_AMP_ATTACK][0],10); 
	osc1_amp_attack[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_AMP_ATTACK][0],10);

	float osc1_amp_decay[4];
	osc1_amp_decay[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_AMP_DECAY][0],10);
	osc1_amp_decay[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_AMP_DECAY][0],10);
	osc1_amp_decay[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_AMP_DECAY][0],10);
	osc1_amp_decay[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_AMP_DECAY][0],10);

	float osc1_amp_sustain[4];
	osc1_amp_sustain[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_AMP_SUSTAIN][0];
	osc1_amp_sustain[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_AMP_SUSTAIN][0];
	osc1_amp_sustain[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_AMP_SUSTAIN][0];
	osc1_amp_sustain[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_AMP_SUSTAIN][0];

	float osc1_amp_release[4];
	osc1_amp_release[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_AMP_RELEASE][0],10);
	osc1_amp_release[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_AMP_RELEASE][0],10);
	osc1_amp_release[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_AMP_RELEASE][0],10);
	osc1_amp_release[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_AMP_RELEASE][0],10);


	float osc1_pitch_attack[4];
	osc1_pitch_attack[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_PITCH_ATTACK][0],10); 
	osc1_pitch_attack[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_PITCH_ATTACK][0],10); 
	osc1_pitch_attack[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_PITCH_ATTACK][0],10); 
	osc1_pitch_attack[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_PITCH_ATTACK][0],10); 

	float osc1_pitch_decay[4];
	osc1_pitch_decay[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_PITCH_DECAY][0],10);
	osc1_pitch_decay[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_PITCH_DECAY][0],10);
	osc1_pitch_decay[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_PITCH_DECAY][0],10);
	osc1_pitch_decay[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_PITCH_DECAY][0],10);

	float osc1_pitch_sustain[4];
	osc1_pitch_sustain[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_PITCH_SUSTAIN][0];
	osc1_pitch_sustain[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_PITCH_SUSTAIN][0];
	osc1_pitch_sustain[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN][0];
	osc1_pitch_sustain[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN][0];

	float osc1_pitch_release[4];
	osc1_pitch_release[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC1_PITCH_RELEASE][0],10);
	osc1_pitch_release[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC1_PITCH_RELEASE][0],10);
	osc1_pitch_release[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC1_PITCH_RELEASE][0],10);
	osc1_pitch_release[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC1_PITCH_RELEASE][0],10);


	
	float osc1_pitch[4];
	osc1_pitch[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_PITCH][0] * 128;
	osc1_pitch[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_PITCH][0] * 128;
	osc1_pitch[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_PITCH][0] * 128;
	osc1_pitch[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_PITCH][0] * 128;

	float osc1_noise[4];	
	osc1_noise[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_NOISE][0];
	osc1_noise[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_NOISE][0];
	osc1_noise[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_NOISE][0];
	osc1_noise[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_NOISE][0];

	float osc1_volume[4];
	osc1_volume[0] = stegosaurus_self->params[stegosaurus_KICK_OSC1_VOLUME][0];
	osc1_volume[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC1_VOLUME][0];
	osc1_volume[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC1_VOLUME][0];
	osc1_volume[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC1_VOLUME][0];

	//-------------------------

	bool osc2_active[4];
	osc2_active[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_ACTIVE][0];
	osc2_active[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_ACTIVE][0];
	osc2_active[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_ACTIVE][0];
	osc2_active[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_ACTIVE][0];

	float osc2_amp_attack[4];
	osc2_amp_attack[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_AMP_ATTACK][0],10); 
	osc2_amp_attack[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_AMP_ATTACK][0],10); 
	osc2_amp_attack[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_AMP_ATTACK][0],10); 
	osc2_amp_attack[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_AMP_ATTACK][0],10); 

	float osc2_amp_decay[4];
	osc2_amp_decay[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_AMP_DECAY][0],10);
	osc2_amp_decay[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_AMP_DECAY][0],10);
	osc2_amp_decay[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_AMP_DECAY][0],10);
	osc2_amp_decay[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_AMP_DECAY][0],10);

	float osc2_amp_sustain[4];
	osc2_amp_sustain[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_AMP_SUSTAIN][0];
	osc2_amp_sustain[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_AMP_SUSTAIN][0];
	osc2_amp_sustain[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_AMP_SUSTAIN][0];
	osc2_amp_sustain[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_AMP_SUSTAIN][0];

	float osc2_amp_release[4];
	osc2_amp_release[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_AMP_RELEASE][0],10);
	osc2_amp_release[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_AMP_RELEASE][0],10);
	osc2_amp_release[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_AMP_RELEASE][0],10);
	osc2_amp_release[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_AMP_RELEASE][0],10);

	float osc2_pitch_attack[4];
	osc2_pitch_attack[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_PITCH_ATTACK][0],10); 
	osc2_pitch_attack[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_PITCH_ATTACK][0],10); 
	osc2_pitch_attack[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_PITCH_ATTACK][0],10); 
	osc2_pitch_attack[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_PITCH_ATTACK][0],10); 

	float osc2_pitch_decay[4];
	osc2_pitch_decay[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_PITCH_DECAY][0],10);
	osc2_pitch_decay[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_PITCH_DECAY][0],10);
	osc2_pitch_decay[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_PITCH_DECAY][0],10);
	osc2_pitch_decay[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_PITCH_DECAY][0],10);

	float osc2_pitch_sustain[4];
	osc2_pitch_sustain[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_PITCH_SUSTAIN][0];
	osc2_pitch_sustain[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_PITCH_SUSTAIN][0];
	osc2_pitch_sustain[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN][0];
	osc2_pitch_sustain[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN][0];

	float osc2_pitch_release[4];
	osc2_pitch_release[0] = fast_pow(1-stegosaurus_self->params[stegosaurus_KICK_OSC2_PITCH_RELEASE][0],10);
	osc2_pitch_release[1] = fast_pow(1-stegosaurus_self->params[stegosaurus_SNARE_OSC2_PITCH_RELEASE][0],10);
	osc2_pitch_release[2] = fast_pow(1-stegosaurus_self->params[stegosaurus_CLHAT_OSC2_PITCH_RELEASE][0],10);
	osc2_pitch_release[3] = fast_pow(1-stegosaurus_self->params[stegosaurus_OPHAT_OSC2_PITCH_RELEASE][0],10);


	float osc2_pitch[4];
	osc2_pitch[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_PITCH][0] * 128;
	osc2_pitch[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_PITCH][0] * 128;
	osc2_pitch[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_PITCH][0] * 128;
	osc2_pitch[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_PITCH][0] * 128;

	float osc2_noise[4];
	osc2_noise[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_NOISE][0];
	osc2_noise[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_NOISE][0];
	osc2_noise[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_NOISE][0];
	osc2_noise[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_NOISE][0];

	float osc2_volume[4];
	osc2_volume[0] = stegosaurus_self->params[stegosaurus_KICK_OSC2_VOLUME][0];
	osc2_volume[1] = stegosaurus_self->params[stegosaurus_SNARE_OSC2_VOLUME][0];
	osc2_volume[2] = stegosaurus_self->params[stegosaurus_CLHAT_OSC2_VOLUME][0];
	osc2_volume[3] = stegosaurus_self->params[stegosaurus_OPHAT_OSC2_VOLUME][0];

	int out_select[4];
	out_select[0] = stegosaurus_self->params[stegosaurus_OUT_SELECTOR_1][0];
	out_select[1] = stegosaurus_self->params[stegosaurus_OUT_SELECTOR_2][0];
	out_select[2] = stegosaurus_self->params[stegosaurus_OUT_SELECTOR_3][0];
	out_select[3] = stegosaurus_self->params[stegosaurus_OUT_SELECTOR_4][0];

	for (int fr=0; fr<n_frames; fr++)
	{
		float voice_out[4];
		voice_out[0] = 0; voice_out[1] = 0; voice_out[2] = 0; voice_out[3] = 0;


			for (int v=0; v<4; v++)
			{
				if ( voices[v]->active)
				{

					// DO OSCILLATOR 1 ------------------------------------------------------
			
					if (osc1_active[v] && voices[v]->env_osc1_amp_state != ENV_STATE_DORMANT)
					{

						//--------- ADSR AMP
						// ATTACK

						switch (voices[v]->env_osc1_amp_state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v]->env_osc1_amp_level < 1) 
							{
								voices[v]->env_osc1_amp_level += osc1_amp_attack[v];
								if (voices[v]->env_osc1_amp_level >= 1)
								{
									voices[v]->env_osc1_amp_level = 1;
									voices[v]->env_osc1_amp_state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v]->env_osc1_amp_level > osc1_amp_sustain[v]) 
							{
								voices[v]->env_osc1_amp_level -= osc1_amp_decay[v];
									if (voices[v]->env_osc1_amp_level <= osc1_amp_sustain[v])
									{
										voices[v]->env_osc1_amp_level = osc1_amp_sustain[v];
										voices[v]->env_osc1_amp_state = ENV_STATE_WAIT;
									}
							}

							if (voices[v]->env_osc1_amp_level < osc1_amp_sustain[v] ) 
							{
								voices[v]->env_osc1_amp_level += osc1_amp_decay[v];
									if (voices[v]->env_osc1_amp_level >= osc1_amp_sustain[v] )
									{
										voices[v]->env_osc1_amp_level = osc1_amp_sustain[v];
										voices[v]->env_osc1_amp_state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v]->env_osc1_amp_level > 0) 
								{
									voices[v]->env_osc1_amp_level -= osc1_amp_release[v];
									if (voices[v]->env_osc1_amp_level <= 0)
									{
										voices[v]->env_osc1_amp_level = 0;
										voices[v]->env_osc1_amp_state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}

			

					float env_osc1_amp_level_db = (voices[v]->env_osc1_amp_level * voices[v]->env_osc1_amp_level * voices[v]->env_osc1_amp_level) * master_volume; 

						//--------- ADSR pitch
						// ATTACK

						switch (voices[v]->env_osc1_pitch_state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v]->env_osc1_pitch_level < 1) 
							{
								voices[v]->env_osc1_pitch_level += osc1_pitch_attack[v];
								if (voices[v]->env_osc1_pitch_level >= 1)
								{
									voices[v]->env_osc1_pitch_level = 1;
									voices[v]->env_osc1_pitch_state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v]->env_osc1_pitch_level > osc1_pitch_sustain[v]) 
							{
								voices[v]->env_osc1_pitch_level -= osc1_pitch_decay[v];
									if (voices[v]->env_osc1_pitch_level <= osc1_pitch_sustain[v] )
									{
										voices[v]->env_osc1_pitch_level = osc1_pitch_sustain[v];
										voices[v]->env_osc1_pitch_state = ENV_STATE_WAIT;
									}
							}

							if (voices[v]->env_osc1_pitch_level < osc1_pitch_sustain[v]) 
							{
								voices[v]->env_osc1_pitch_level += osc1_pitch_decay[v];
									if (voices[v]->env_osc1_pitch_level >= osc1_pitch_sustain[v] )
									{
										voices[v]->env_osc1_pitch_level = osc1_pitch_sustain[v];
										voices[v]->env_osc1_pitch_state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v]->env_osc1_pitch_level > 0) 
								{
									voices[v]->env_osc1_pitch_level -= osc1_pitch_release[v];
									if (voices[v]->env_osc1_pitch_level <= 0)
									{
										voices[v]->env_osc1_pitch_level = 0;
										voices[v]->env_osc1_pitch_state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}

					float env_osc1_pitch_level_db = voices[v]->env_osc1_pitch_level; 

						voices[v]->osc[0].wave_mix = 0;	

						voices[v]->osc[0].wave1_number = 0;
						voices[v]->osc[0].wave2_number = 0;
						float new_pitch = osc1_pitch[v]  * env_osc1_pitch_level_db;
						float pitch_mod = noise_tick() * osc1_noise[v];
						new_pitch += pitch_mod * 512;	
						if (new_pitch < 0) new_pitch = 0;
						voices[v]->osc[0].frequency = new_pitch;

						voice_out[v] += (osc1_volume[v] * (wavetable_tick( &voices[v]->osc[0] ) * 0.5) * env_osc1_amp_level_db);	
					}

					// DO OSCILLATOR 2 ------------------------------------------------------
			
					if (osc2_active[v] && voices[v]->env_osc2_amp_state != ENV_STATE_DORMANT)
					{

						//--------- ADSR AMP
						// ATTACK

						switch (voices[v]->env_osc2_amp_state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v]->env_osc2_amp_level < 1) 
							{
								voices[v]->env_osc2_amp_level += osc2_amp_attack[v];
								if (voices[v]->env_osc2_amp_level >= 1)
								{
									voices[v]->env_osc2_amp_level = 1;
									voices[v]->env_osc2_amp_state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v]->env_osc2_amp_level > osc2_amp_sustain[v] ) 
							{
								voices[v]->env_osc2_amp_level -= osc2_amp_decay[v];
									if (voices[v]->env_osc2_amp_level <= osc2_amp_sustain[v])
									{
										voices[v]->env_osc2_amp_level = osc2_amp_sustain[v];
										voices[v]->env_osc2_amp_state = ENV_STATE_WAIT;
									}
							}

							if (voices[v]->env_osc2_amp_level < osc2_amp_sustain[v] ) 
							{
								voices[v]->env_osc2_amp_level += osc2_amp_decay[v];
									if (voices[v]->env_osc2_amp_level >= osc2_amp_sustain[v] )
									{
										voices[v]->env_osc2_amp_level = osc2_amp_sustain[v];
										voices[v]->env_osc2_amp_state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v]->env_osc2_amp_level > 0) 
								{
									voices[v]->env_osc2_amp_level -= osc2_amp_release[v] ;
									if (voices[v]->env_osc2_amp_level <= 0)
									{
										voices[v]->env_osc2_amp_level = 0;
										voices[v]->env_osc2_amp_state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}

			

						float env_osc2_amp_level_db = voices[v]->volume * (voices[v]->env_osc2_amp_level * voices[v]->env_osc2_amp_level * voices[v]->env_osc2_amp_level) * master_volume; 

						//--------- ADSR pitch
						// ATTACK

						switch (voices[v]->env_osc2_pitch_state)
						{


						case ENV_STATE_ATTACK:
							if (voices[v]->env_osc2_pitch_level < 1) 
							{
								voices[v]->env_osc2_pitch_level += osc2_pitch_attack[v];
								if (voices[v]->env_osc2_pitch_level >= 1)
								{
									voices[v]->env_osc2_pitch_level = 1;
									voices[v]->env_osc2_pitch_state = ENV_STATE_DECAY;
								}
							}
							break;

						// DECAY / SUSTAIN

						case ENV_STATE_DECAY:
						{	
							if (voices[v]->env_osc2_pitch_level > osc2_pitch_sustain[v] ) 
							{
								voices[v]->env_osc2_pitch_level -= osc2_pitch_decay[v];
									if (voices[v]->env_osc2_pitch_level <= osc2_pitch_sustain[v])
									{
										voices[v]->env_osc2_pitch_level = osc2_pitch_sustain[v];
										voices[v]->env_osc2_pitch_state = ENV_STATE_WAIT;
									}
							}

							if (voices[v]->env_osc2_pitch_level < osc2_pitch_sustain[v] ) 
							{
								voices[v]->env_osc2_pitch_level += osc2_pitch_decay[v];
									if (voices[v]->env_osc2_pitch_level >= osc2_pitch_sustain[v] )
									{
										voices[v]->env_osc2_pitch_level = osc2_pitch_sustain[v];
										voices[v]->env_osc2_pitch_state = ENV_STATE_WAIT;
									}
							}
							break;

							// RELEASE

							case ENV_STATE_RELEASE:
								if (voices[v]->env_osc2_pitch_level > 0) 
								{
									voices[v]->env_osc2_pitch_level -= osc2_pitch_release[v];
									if (voices[v]->env_osc2_pitch_level <= 0)
									{
										voices[v]->env_osc2_pitch_level = 0;
										voices[v]->env_osc2_pitch_state = ENV_STATE_DORMANT;
									}
								}
								break;
							}
						}

						float env_osc2_pitch_level_db = voices[v]->env_osc2_pitch_level; 

						voices[v]->osc[1].wave_mix = 0;	

						voices[v]->osc[1].wave1_number = 0;
						voices[v]->osc[1].wave2_number = 0;
						float new_pitch = osc2_pitch[v]  * env_osc2_pitch_level_db;
						float pitch_mod = noise_tick() * osc2_noise[v];
						new_pitch += pitch_mod * 512;	
						if (new_pitch < 0) new_pitch = 0;
						voices[v]->osc[1].frequency = new_pitch;

						voice_out[v] += voices[v]->volume *  (osc2_volume[v] * (wavetable_tick( &voices[v]->osc[1] ) * 0.5) * env_osc2_amp_level_db);	


					}	

				}
			}

		// OUTPUT VOICE ONE - KICK

		if (out_select[0] == 0)
		{
			out_one[fr] += voice_out[0];
			out_two[fr] += voice_out[0];
		}			
		else if (out_select[0] == 1)
		{
			out_one[fr] += voice_out[0];
		}
		else if (out_select[0] == 2)
		{
			out_two[fr] += voice_out[0];
		}
		else if (out_select[0] == 3)
		{
			out_three[fr] += voice_out[0];
		}
		else if (out_select[0] == 4)
		{
			out_four[fr] += voice_out[0];
		}


		// OUTPUT VOICE TWO - SNARE

		if (out_select[1] == 0)
		{
			out_one[fr] += voice_out[1];
			out_two[fr] += voice_out[1];
		}			
		else if (out_select[1] == 1)
		{
			out_one[fr] += voice_out[1];
		}
		else if (out_select[1] == 2)
		{
			out_two[fr] += voice_out[1];
		}
		else if (out_select[1] == 3)
		{
			out_three[fr] += voice_out[1];
		}
		else if (out_select[1] == 4)
		{
			out_four[fr] += voice_out[1];
		}

		// OUTPUT VOICE THREE - CLOSED HAT

		if (out_select[2] == 0)
		{
			out_one[fr] += voice_out[2];
			out_two[fr] += voice_out[2];
		}			
		else if (out_select[2] == 1)
		{
			out_one[fr] += voice_out[2];
		}
		else if (out_select[2] == 2)
		{
			out_two[fr] += voice_out[2];
		}
		else if (out_select[2] == 3)
		{
			out_three[fr] += voice_out[2];
		}
		else if (out_select[2] == 4)
		{
			out_four[fr] += voice_out[2];
		}

		// OUTPUT VOICE FOUR - OPEN HAT

		if (out_select[3] == 0)
		{
			out_one[fr] += voice_out[3];
			out_two[fr] += voice_out[3];
		}			
		else if (out_select[3] == 1)
		{
			out_one[fr] += voice_out[3];
		}
		else if (out_select[3] == 2)
		{
			out_two[fr] += voice_out[3];
		}
		else if (out_select[3] == 3)
		{
			out_three[fr] += voice_out[3];
		}
		else if (out_select[3] == 4)
		{
			out_four[fr] += voice_out[3];
		}

	}
}



void stegosaurus_synth_note_on(int note, int volume)
{


	if (note == 60)
	{
		printf("Kick triggered..\n");
		voices[0]->active = true;
		voices[0]->env_osc1_amp_state = ENV_STATE_ATTACK;
		voices[0]->env_osc1_pitch_state = ENV_STATE_ATTACK;
		voices[0]->env_osc1_amp_level = 0;
		voices[0]->env_osc1_pitch_level = 0;
		voices[0]->env_osc2_amp_state = ENV_STATE_ATTACK;
		voices[0]->env_osc2_pitch_state = ENV_STATE_ATTACK;
		voices[0]->env_osc2_amp_level = 0;
		voices[0]->env_osc2_pitch_level = 0;
		voices[0]->frequency = note;
		voices[0]->volume = (float)volume/128 ;
		keys[note] = 0;
	}

	if (note == 62)
	{
		printf("Snare triggered..\n");
		voices[1]->active = true;
		voices[1]->env_osc1_amp_state = ENV_STATE_ATTACK;
		voices[1]->env_osc1_pitch_state = ENV_STATE_ATTACK;
		voices[1]->env_osc1_amp_level = 0;
		voices[1]->env_osc1_pitch_level = 0;
		voices[1]->env_osc2_amp_state = ENV_STATE_ATTACK;
		voices[1]->env_osc2_pitch_state = ENV_STATE_ATTACK;
		voices[1]->env_osc2_amp_level = 0;
		voices[1]->env_osc2_pitch_level = 0;
		voices[1]->frequency = note;
		voices[1]->volume = (float)volume/128 ;
		keys[note] = 1;
	}

	if (note == 64)
	{
		printf("Closed hat triggered..\n");
		voices[2]->active = true;
		voices[2]->env_osc1_amp_state = ENV_STATE_ATTACK;
		voices[2]->env_osc1_pitch_state = ENV_STATE_ATTACK;
		voices[2]->env_osc1_amp_level = 0;
		voices[2]->env_osc1_pitch_level = 0;
		voices[2]->env_osc2_amp_state = ENV_STATE_ATTACK;
		voices[2]->env_osc2_pitch_state = ENV_STATE_ATTACK;
		voices[2]->env_osc2_amp_level = 0;
		voices[2]->env_osc2_pitch_level = 0;
		voices[2]->frequency = note;
		voices[2]->volume = (float)volume/128 ;
		keys[note] = 2;
	}

	if (note == 65)
	{
		printf("Open hat triggered..\n");
		voices[3]->active = true;
		voices[3]->env_osc1_amp_state = ENV_STATE_ATTACK;
		voices[3]->env_osc1_pitch_state = ENV_STATE_ATTACK;
		voices[3]->env_osc1_amp_level = 0;
		voices[3]->env_osc1_pitch_level = 0;
		voices[3]->env_osc2_amp_state = ENV_STATE_ATTACK;
		voices[3]->env_osc2_pitch_state = ENV_STATE_ATTACK;
		voices[3]->env_osc2_amp_level = 0;
		voices[3]->env_osc2_pitch_level = 0;
		voices[3]->frequency = note;
		voices[3]->volume = (float)volume/128 ;
		keys[note] = 3;
	}
}


void stegosaurus_synth_note_off(int note)
{
	int v = keys[note];

	if (v == -1) return;

	voices[v]->env_osc1_amp_state = ENV_STATE_RELEASE;
	voices[v]->env_osc1_pitch_state = ENV_STATE_RELEASE;
	voices[v]->env_osc2_amp_state = ENV_STATE_RELEASE;
	voices[v]->env_osc2_pitch_state = ENV_STATE_RELEASE;
	voices[v]->env_filt_state = ENV_STATE_RELEASE;

	keys[note] = -1;

}


//----------------------------------------------------------------------------------------

int stegosaurus_synth_get_number_of_voices_playing()
{
	int number_of_voices_playing = 0;
	if (voices[0]->active) number_of_voices_playing++;
	if (voices[1]->active) number_of_voices_playing++;
	if (voices[2]->active) number_of_voices_playing++;
	if (voices[3]->active) number_of_voices_playing++;
	
	return number_of_voices_playing;
}

//----------------------------------------------------------------------------------------

void stegosaurus_synth_set_osc_waves(int osc, int wave1, int wave2)
{
	for (int v=0; v<4; v++)
	{
		voices[v]->osc[osc].wave1_number = wave1;
		voices[v]->osc[osc].wave2_number = wave2;
	}
}







