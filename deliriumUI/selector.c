
#include "deliriumUI.h"
#include <math.h>


void setValuedeliriumUIselector(deliriumUI* deliriumUI_window, int widgetNumber, float _value)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;
	deliriumUIWidgets[widgetNumber].values[0] = (bool)_value;
	deliriumUIWidgets[widgetNumber].pressed = (bool)_value;
}


void displaydeliriumUIselector(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	float x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	float y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	float w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	float h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;

	float sx = x;
	float sy = y;
	float sw = w;
	float sh = h * 0.8;

	cairo_set_line_width(cr, 4);

	//---------------------

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.5);
	cairo_paint(cr);



	// Draw label

	deliriumUIWidgets[widgetNumber].label = "WIBBLE";

	if (deliriumUIWidgets[widgetNumber].values[0] == 0)
		deliriumUIWidgets[widgetNumber].label = "OUTS: 1+2";

	if (deliriumUIWidgets[widgetNumber].values[0] == 1)
		deliriumUIWidgets[widgetNumber].label = "OUTS: 1";

	if (deliriumUIWidgets[widgetNumber].values[0] == 2)
		deliriumUIWidgets[widgetNumber].label = "OUTS: 2";

	if (deliriumUIWidgets[widgetNumber].values[0] == 3)
		deliriumUIWidgets[widgetNumber].label = "OUTS: 3";

	if (deliriumUIWidgets[widgetNumber].values[0] == 4)
		deliriumUIWidgets[widgetNumber].label = "OUTS: 4";

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, h / 5);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              (x + w / 2) - extents.width / 2,
	              (y + h / 2) - extents.height);

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);
	}

	cairo_show_text(cr, deliriumUIWidgets[widgetNumber].label);


	cairo_reset_clip(cr);
}
