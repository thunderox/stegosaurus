
#include<stdlib.h>
#include"uris.h"
#include"lv2/lv2plug.in/ns/lv2core/lv2.h"
#include"lv2/lv2plug.in/ns/ext/atom/forge.h"

typedef enum {
	stegosaurus_MIDI_IN=0,
	stegosaurus_OUT_ONE=1,
	stegosaurus_OUT_TWO=2,
	stegosaurus_VOLUME=3,

	stegosaurus_KICK_OSC1_ACTIVE=4,
	stegosaurus_KICK_OSC1_NOISE=5,
	stegosaurus_KICK_OSC1_PITCH=6,
	stegosaurus_KICK_OSC1_AMP_ATTACK=7,
	stegosaurus_KICK_OSC1_AMP_DECAY=8,
	stegosaurus_KICK_OSC1_AMP_SUSTAIN=9,
	stegosaurus_KICK_OSC1_AMP_RELEASE=10,
	stegosaurus_KICK_OSC1_PITCH_ATTACK=11,
	stegosaurus_KICK_OSC1_PITCH_DECAY=12,
	stegosaurus_KICK_OSC1_PITCH_SUSTAIN=13,
	stegosaurus_KICK_OSC1_PITCH_RELEASE=14,
	stegosaurus_KICK_OSC1_VOLUME=15,

	stegosaurus_KICK_OSC2_ACTIVE=16,
	stegosaurus_KICK_OSC2_NOISE=17,
	stegosaurus_KICK_OSC2_PITCH=18,
	stegosaurus_KICK_OSC2_AMP_ATTACK=19,
	stegosaurus_KICK_OSC2_AMP_DECAY=20,
	stegosaurus_KICK_OSC2_AMP_SUSTAIN=21,
	stegosaurus_KICK_OSC2_AMP_RELEASE=22,
	stegosaurus_KICK_OSC2_PITCH_ATTACK=23,
	stegosaurus_KICK_OSC2_PITCH_DECAY=24,
	stegosaurus_KICK_OSC2_PITCH_SUSTAIN=25,
	stegosaurus_KICK_OSC2_PITCH_RELEASE=26,
	stegosaurus_KICK_OSC2_VOLUME=27,


	stegosaurus_SNARE_OSC1_ACTIVE=28,
	stegosaurus_SNARE_OSC1_NOISE=29,
	stegosaurus_SNARE_OSC1_PITCH=30,
	stegosaurus_SNARE_OSC1_AMP_ATTACK=31,
	stegosaurus_SNARE_OSC1_AMP_DECAY=32,
	stegosaurus_SNARE_OSC1_AMP_SUSTAIN=33,
	stegosaurus_SNARE_OSC1_AMP_RELEASE=34,
	stegosaurus_SNARE_OSC1_PITCH_ATTACK=35,
	stegosaurus_SNARE_OSC1_PITCH_DECAY=36,
	stegosaurus_SNARE_OSC1_PITCH_SUSTAIN=37,
	stegosaurus_SNARE_OSC1_PITCH_RELEASE=38,
	stegosaurus_SNARE_OSC1_VOLUME=39,

	stegosaurus_SNARE_OSC2_ACTIVE=40,
	stegosaurus_SNARE_OSC2_NOISE=41,
	stegosaurus_SNARE_OSC2_PITCH=42,
	stegosaurus_SNARE_OSC2_AMP_ATTACK=43,
	stegosaurus_SNARE_OSC2_AMP_DECAY=44,
	stegosaurus_SNARE_OSC2_AMP_SUSTAIN=45,
	stegosaurus_SNARE_OSC2_AMP_RELEASE=46,
	stegosaurus_SNARE_OSC2_PITCH_ATTACK=47,
	stegosaurus_SNARE_OSC2_PITCH_DECAY=48,
	stegosaurus_SNARE_OSC2_PITCH_SUSTAIN=49,
	stegosaurus_SNARE_OSC2_PITCH_RELEASE=50,
	stegosaurus_SNARE_OSC2_VOLUME=51,


	stegosaurus_CLHAT_OSC1_ACTIVE=52,
	stegosaurus_CLHAT_OSC1_NOISE=53,
	stegosaurus_CLHAT_OSC1_PITCH=54,
	stegosaurus_CLHAT_OSC1_AMP_ATTACK=55,
	stegosaurus_CLHAT_OSC1_AMP_DECAY=56,
	stegosaurus_CLHAT_OSC1_AMP_SUSTAIN=57,
	stegosaurus_CLHAT_OSC1_AMP_RELEASE=58,
	stegosaurus_CLHAT_OSC1_PITCH_ATTACK=59,
	stegosaurus_CLHAT_OSC1_PITCH_DECAY=60,
	stegosaurus_CLHAT_OSC1_PITCH_SUSTAIN=61,
	stegosaurus_CLHAT_OSC1_PITCH_RELEASE=62,
	stegosaurus_CLHAT_OSC1_VOLUME=63,

	stegosaurus_CLHAT_OSC2_ACTIVE=64,
	stegosaurus_CLHAT_OSC2_NOISE=65,
	stegosaurus_CLHAT_OSC2_PITCH=66,
	stegosaurus_CLHAT_OSC2_AMP_ATTACK=67,
	stegosaurus_CLHAT_OSC2_AMP_DECAY=68,
	stegosaurus_CLHAT_OSC2_AMP_SUSTAIN=69,
	stegosaurus_CLHAT_OSC2_AMP_RELEASE=70,
	stegosaurus_CLHAT_OSC2_PITCH_ATTACK=71,
	stegosaurus_CLHAT_OSC2_PITCH_DECAY=72,
	stegosaurus_CLHAT_OSC2_PITCH_SUSTAIN=73,
	stegosaurus_CLHAT_OSC2_PITCH_RELEASE=74,
	stegosaurus_CLHAT_OSC2_VOLUME=75,


	stegosaurus_OPHAT_OSC1_ACTIVE=76,
	stegosaurus_OPHAT_OSC1_NOISE=77,
	stegosaurus_OPHAT_OSC1_PITCH=78,
	stegosaurus_OPHAT_OSC1_AMP_ATTACK=79,
	stegosaurus_OPHAT_OSC1_AMP_DECAY=80,
	stegosaurus_OPHAT_OSC1_AMP_SUSTAIN=81,
	stegosaurus_OPHAT_OSC1_AMP_RELEASE=82,
	stegosaurus_OPHAT_OSC1_PITCH_ATTACK=83,
	stegosaurus_OPHAT_OSC1_PITCH_DECAY=84,
	stegosaurus_OPHAT_OSC1_PITCH_SUSTAIN=85,
	stegosaurus_OPHAT_OSC1_PITCH_RELEASE=86,
	stegosaurus_OPHAT_OSC1_VOLUME=87,

	stegosaurus_OPHAT_OSC2_ACTIVE=88,
	stegosaurus_OPHAT_OSC2_NOISE=89,
	stegosaurus_OPHAT_OSC2_PITCH=90,
	stegosaurus_OPHAT_OSC2_AMP_ATTACK=91,
	stegosaurus_OPHAT_OSC2_AMP_DECAY=92,
	stegosaurus_OPHAT_OSC2_AMP_SUSTAIN=93,
	stegosaurus_OPHAT_OSC2_AMP_RELEASE=94,
	stegosaurus_OPHAT_OSC2_PITCH_ATTACK=95,
	stegosaurus_OPHAT_OSC2_PITCH_DECAY=96,
	stegosaurus_OPHAT_OSC2_PITCH_SUSTAIN=97,
	stegosaurus_OPHAT_OSC2_PITCH_RELEASE=98,
	stegosaurus_OPHAT_OSC2_VOLUME=99,
	stegosaurus_OUT_THREE=100,
	stegosaurus_OUT_FOUR=101,
	stegosaurus_OUT_SELECTOR_1=102,
	stegosaurus_OUT_SELECTOR_2=103,
	stegosaurus_OUT_SELECTOR_3=104,
	stegosaurus_OUT_SELECTOR_4=105


} PortIndex;

typedef struct {
	float* out_one;
	float* out_two;
	float* out_three;
	float* out_four;
	
	float* params[160];

	stegosaurusURIs uris;
	LV2_Atom_Forge forge;
	LV2_URID_Map* map;
	char *bundle_path;

	LV2_Atom_Sequence* midi_in;


} stegosaurus;







