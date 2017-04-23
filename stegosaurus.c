/*
  Copyright 2015 Nick Bailey
*/

#include "stegosaurus_synth.h"


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
	stegosaurus* self = (stegosaurus*)malloc(sizeof(stegosaurus));

	/* Get host features */
	for (int i = 0; features[i]; ++i) {
		if (!strcmp(features[i]->URI, LV2_URID__map)) {
			self->map = (LV2_URID_Map*)features[i]->data;
		}  
	}

	if (!self->map) {
		printf( "Missing feature urid:map.\n");	}

	/* Map URIs and initialise forge */
	map_stegosaurus_uris(self->map, &self->uris);
	lv2_atom_forge_init(&self->forge, self->map);

	// store the bundle_path string to "self"
	self->bundle_path = (char*)malloc(strlen(bundle_path)+1);
	memcpy (self->bundle_path,bundle_path,strlen(bundle_path)+1);
	stegosaurus_self = self;

	stegosaurus_synth_init(rate);

	return (LV2_Handle)self;
}

/** Connect a port to a buffer (audio thread, must be RT safe). */
static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
	stegosaurus* self = (stegosaurus*)instance;

	if ( (PortIndex)port == stegosaurus_MIDI_IN)
	{
		self->midi_in = (LV2_Atom_Sequence*)data;
		return;
	}

	if ( (PortIndex)port > stegosaurus_MIDI_IN) self->params[(PortIndex)port] = (float*)data;
	{
		return;
	}


}

/** Initialise and prepare the plugin instance for running. */
static void
activate(LV2_Handle instance)
{
	/* Nothing to do here in this trivial mostly stateless plugin. */
}

#define DB_CO(g) ((g) > -90.0f ? powf(10.0f, (g) * 0.05f) : 0.0f)

/** Process a block of audio (audio thread, must be RT safe). */
static void
run(LV2_Handle instance, uint32_t n_samples)
{
	stegosaurus* self = (stegosaurus*)instance;

	stegosaurusURIs* uris = &self->uris;

	memset( self->params[stegosaurus_OUT_LEFT], 0, sizeof(double)*(n_samples*0.5) );
	memset( self->params[stegosaurus_OUT_RIGHT], 0, sizeof(double)*(n_samples*0.5) );

	// const float coef = DB_CO(

	// ---------------------- MIDI EVENTS ------------------------------

	LV2_ATOM_SEQUENCE_FOREACH(self->midi_in, ev)
	{

		printf("Some sort of atom event detected.........");

		if (ev->body.type == uris->midi_Event)
		{
			uint8_t* const data = (uint8_t* const)(ev + 1);

			printf("Event detected - %d - %d - %d \n", data[0], data[1], data[2] );

			if (data[0] == 0x99 && data[2] > 0)
			{
				stegosaurus_synth_note_on( data[1], data[2] );
			}

			if (data[0] == 0x89 || data[0] == 0x99 && data[2] == 0)
			{
				stegosaurus_synth_note_off( data[1] );
			}
		}
	}

	// ------------------ DO AUDIO -------------------------------------

	if (stegosaurus_synth_get_number_of_voices_playing() > 0)
	{

		stegosaurus_synth_play(self->params[stegosaurus_OUT_LEFT], self->params[stegosaurus_OUT_RIGHT], n_samples); 
	}
}

/** Finish running (counterpart to activate()). */
static void
deactivate(LV2_Handle instance)
{
	/* Nothing to do here in this trivial mostly stateless plugin. */
}

/** Destroy a plugin instance (counterpart to instantiate()). */
static void
cleanup(LV2_Handle instance)
{
	free(instance);
}

/** Return extension data provided by the plugin. */
static const void*
extension_data(const char* uri)
{
	return NULL;  /* This plugin has no extension data. */
}

/** The LV2_Descriptor for this plugin. */
static const LV2_Descriptor descriptor = {
	stegosaurus_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

/** Entry point, the host will call this function to access descriptors. */
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}









