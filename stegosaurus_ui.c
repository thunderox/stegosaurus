/*
  Copyright 2015 Nick Bailey
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stegosaurus.h"

#include <cairo/cairo.h>

#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "deliriumUI/deliriumUI.h"

#define stegosaurus_UI_URI "http://nickbailey.co.nr/plugins/stegosaurus#ui"

typedef struct {
	PuglView*            view;
	LV2UI_Write_Function write;
	LV2UI_Controller     controller;
	int                  width;
	int                  height;
	deliriumUI	     deliriumUI_window;
	float                brightness;
	bool                 exit;
} stegosaurusUI;

static void
onReshape(PuglView* view, int width, int height)
{

	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);

	setDeliriumUICurrentWindowSize(&self->deliriumUI_window, width, height);
}


static void
onDisplay(PuglView* view)
{
	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	displayAllDeliriumUIWidgets(&self->deliriumUI_window,cr);
}

static void
onKeyboard(PuglView* view, bool press, uint32_t key)
{
	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	if (press) {

		if (key==27)
		{
			self->exit = true;
		}


		if (key=='r')
		{
			displayAllDeliriumUIWidgets(&self->deliriumUI_window,cr);
		}

		fprintf(stderr, "Keyboard press %c\n", key);
	} else {
		fprintf(stderr, "Keyboard release %c\n", key);
	}
}

static void
onMotion(PuglView* view, int x, int y)
{
	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	// fprintf(stderr, "Motion: %d,%d\n", x, y);
	isMouseOverDeliriumUIWidget(&self->deliriumUI_window, cr, x, y);
}

static void
onMouse(PuglView* view, int button, bool press, int x, int y)
{
	if (press == true && button == 0 || button > 15) return;

	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	self->deliriumUI_window.mouseButton[button] = press;

	if (press == true && button == 3)
		zeroValue(&self->deliriumUI_window, cr);

	if (press == true && button == 1)
		hasMouseClickedDeliriumUIWidget(&self->deliriumUI_window, x, y);
		else
		hasMouseClickedDeliriumUIWidget(&self->deliriumUI_window, -1,-1);

}

static void
onScroll(PuglView* view, int x, int y, float dx, float dy)
{
	stegosaurusUI* self = (stegosaurusUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	if (dy!=0) 
	{

		if (dy == -1)
		{
			decValue(&self->deliriumUI_window, cr);
		}

		if (dy == 1)
		{
			incValue(&self->deliriumUI_window, cr);
		}
	}
}


static LV2UI_Handle
instantiate(const LV2UI_Descriptor*   descriptor,
            const char*               plugin_uri,
            const char*               bundle_path,
            LV2UI_Write_Function      write_function,
            LV2UI_Controller          controller,
            LV2UI_Widget*             widget,
            const LV2_Feature* const* features)
{
	stegosaurusUI* self = (stegosaurusUI*)malloc(sizeof(stegosaurusUI));
	if (!self) {
		return NULL;
	}

	self->write      = write_function;
	self->controller = controller;
	self->width      = 800;
	self->height     = 360;
	self->exit       = false;	

	self->deliriumUI_window.deliriumUIWidgets = NULL;
	self->deliriumUI_window.numberOfUIWidgets = 1;
	self->deliriumUI_window.currentWidgetNumber = -1;

	for (int b=0; b<16; ++b)
	{
		self->deliriumUI_window.mouseButton[b] = 0;
		self->deliriumUI_window.group_visible[b] = false; 
		self->deliriumUI_window.group_visible[b+16] = false; 
	}

	setDeliriumUIGridSize(&self->deliriumUI_window, self->width, self->height, 16, 12);


	// Get parent window and resize API from features
	PuglNativeWindow parent = 0;
	LV2UI_Resize*    resize = NULL;
	for (int i = 0; features && features[i]; ++i) {
		printf("Feature %s\n", features[i]->URI );
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parent = (PuglNativeWindow)features[i]->data;
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
		}
	}

	if (!parent) {
		fprintf(stderr, "error: stegosaurus_ui: No parent window provided.\n");
		free(self);
		return NULL;
	}


	setControllerDiliriumUI(controller, write_function);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 5, 0,0,2,1,"KICK",-1);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 5, 2,0,2,1,"SNARE",-2);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 5, 4,0,2,1,"CLOSED HAT",-3);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 5, 6,0,2,1,"OPEN HAT",-4);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 1, 0.5,3,1,2,"OSC 1",stegosaurus_KICK_OSC1_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 1.5,2,1,4,"NOISE",stegosaurus_KICK_OSC1_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 2.5,2,1,4,"PITCH",stegosaurus_KICK_OSC1_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1, 4,2,3,3,"AMP",stegosaurus_KICK_OSC1_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1, 7.52,2,3,3,"PITCH",stegosaurus_KICK_OSC1_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 11,2,1,4,"VOL",stegosaurus_KICK_OSC1_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 1, 0.5,8,1,2,"OSC 2",stegosaurus_KICK_OSC2_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 1.5,7,1,4,"NOISE",stegosaurus_KICK_OSC2_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 2.5,7,1,4,"PITCH",stegosaurus_KICK_OSC2_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1, 4,7,3,3,"AMP",stegosaurus_KICK_OSC2_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1, 7.52,7,3,3,"PITCH",stegosaurus_KICK_OSC2_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 11,7,1,4,"VOL", stegosaurus_KICK_OSC2_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 2, 0.5,3,1,2,"OSC 1",stegosaurus_SNARE_OSC1_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 1.5,2,1,4,"NOISE",stegosaurus_SNARE_OSC1_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 2.5,2,1,4,"PITCH",stegosaurus_SNARE_OSC1_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 2, 4,2,3,3,"AMP",stegosaurus_SNARE_OSC1_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 2, 7.52,2,3,3,"PITCH",stegosaurus_SNARE_OSC1_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 11,2,1,4,"VOL",stegosaurus_SNARE_OSC1_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 2, 0.5,8,1,2,"OSC 2",stegosaurus_SNARE_OSC2_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 1.5,7,1,4,"NOISE",stegosaurus_SNARE_OSC2_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 2.5,7,1,4,"PITCH",stegosaurus_SNARE_OSC2_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 2, 4,7,3,3,"AMP",stegosaurus_SNARE_OSC2_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 2, 7.52,7,3,3,"PITCH",stegosaurus_SNARE_OSC2_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2, 11,7,1,4,"VOL", stegosaurus_SNARE_OSC2_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 3, 0.5,3,1,2,"OSC 1",stegosaurus_CLHAT_OSC1_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 1.5,2,1,4,"NOISE",stegosaurus_CLHAT_OSC1_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 2.5,2,1,4,"PITCH",stegosaurus_CLHAT_OSC1_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 3, 4,2,3,3,"AMP",stegosaurus_CLHAT_OSC1_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 3, 7.52,2,3,3,"PITCH",stegosaurus_CLHAT_OSC1_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 11,2,1,4,"VOL",stegosaurus_CLHAT_OSC1_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 3, 0.5,8,1,2,"OSC 2",stegosaurus_CLHAT_OSC2_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 1.5,7,1,4,"NOISE",stegosaurus_CLHAT_OSC2_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 2.5,7,1,4,"PITCH",stegosaurus_CLHAT_OSC2_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 3, 4,7,3,3,"AMP",stegosaurus_CLHAT_OSC2_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 3, 7.52,7,3,3,"PITCH",stegosaurus_CLHAT_OSC2_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 11,7,1,4,"VOL", stegosaurus_CLHAT_OSC2_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 4, 0.5,3,1,2,"OSC 1",stegosaurus_OPHAT_OSC1_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 1.5,2,1,4,"NOISE",stegosaurus_OPHAT_OSC1_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 2.5,2,1,4,"PITCH",stegosaurus_OPHAT_OSC1_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 4, 4,2,3,3,"AMP",stegosaurus_OPHAT_OSC1_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 4, 7.52,2,3,3,"PITCH",stegosaurus_OPHAT_OSC1_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 11,2,1,4,"VOL",stegosaurus_OPHAT_OSC1_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 4, 0.5,8,1,2,"OSC 2",stegosaurus_OPHAT_OSC2_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 1.5,7,1,4,"NOISE",stegosaurus_OPHAT_OSC2_NOISE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 2.5,7,1,4,"PITCH",stegosaurus_OPHAT_OSC2_PITCH);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 4, 4,7,3,3,"AMP",stegosaurus_OPHAT_OSC2_AMP_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 4, 7.52,7,3,3,"PITCH",stegosaurus_OPHAT_OSC2_PITCH_ATTACK);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 4, 11,7,1,4,"VOL", stegosaurus_OPHAT_OSC2_VOLUME);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Knob, 5, 13.5,6,2,4,"VOLUME",stegosaurus_VOLUME);

	self->deliriumUI_window.group_visible[1] = true;
	self->deliriumUI_window.group_visible[2] = false;
	self->deliriumUI_window.group_visible[3] = false;
	self->deliriumUI_window.group_visible[4] = false;
	self->deliriumUI_window.group_visible[5] = true;

	// Set up pugl window

	self->view = puglInit(NULL, NULL);
	puglInitResizable(self->view, false);
	puglInitWindowParent(self->view, parent);
	puglInitWindowSize(self->view, 800,360);
	puglInitContextType(self->view, PUGL_CAIRO);
	puglIgnoreKeyRepeat(self->view, false);

	puglSetDisplayFunc(self->view, onDisplay);
	puglSetReshapeFunc(self->view, onReshape);
	puglSetKeyboardFunc(self->view, onKeyboard);
	puglSetMotionFunc(self->view, onMotion);
	puglSetMouseFunc(self->view, onMouse);
	puglSetScrollFunc(self->view, onScroll);

	puglCreateWindow(self->view, "stegosaurus");
	puglSetHandle(self->view, self);
	puglShowWindow(self->view);

	if (resize) {
		resize->ui_resize(resize->handle, self->width, self->height);
	}

	*widget = (void*)puglGetNativeWindow(self->view);
	return self;
}

static int
idle(LV2UI_Handle handle)
{
	stegosaurusUI* self = (stegosaurusUI*)handle;

	// Silly pulsing animation to check that idle handler is working
	self->brightness = fmod(self->brightness + 0.01, 1.0);
	puglPostRedisplay(self->view);
	puglProcessEvents(self->view);

	return 0;
}

static void
cleanup(LV2UI_Handle handle)
{

	stegosaurusUI* self = (stegosaurusUI*)handle;
	self->exit = true;

	fprintf(stderr, "\nstegosaurus shutting down........\n");

	puglDestroy(self->view);
	free(self);

}

static void
port_event(LV2UI_Handle handle,
           uint32_t     port_index,
           uint32_t     buffer_size,
           uint32_t     format,
           const void*  buffer)
{
	stegosaurusUI* self = (stegosaurusUI*)handle;
	cairo_t* cr = puglGetContext(self->view);
	float value = *(float*)buffer;

	setValue(&self->deliriumUI_window, cr, port_index, value);

	
}

static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) {
		return &idle_iface;
	}
	return NULL;
}

static const LV2UI_Descriptor descriptor = {
	stegosaurus_UI_URI,
	instantiate,
	cleanup,
	port_event,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}








