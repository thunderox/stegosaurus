

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

#include "../../delirium_ui/delirium_ui.hpp"

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::CairoGraphicsContext;
using DGL_NAMESPACE::CairoImage;
using DGL_NAMESPACE::CairoImageButton;
using DGL_NAMESPACE::CairoImageKnob;

class stegosaurus_ui : public UI
{
	public:

		//------------------------------------------------------------------------------------------------------

		stegosaurus_ui() : UI(740, 340)
		{
		
			memset(fParameters, 0, sizeof(float)*kParameterCount);
			memset(fParameters_widget_number, -1, sizeof(int)*(kParameterCount+10));
		
		        setGeometryConstraints(740,340, true, true);


			GUI = Delirium_UI_Init(static_cast<float>(getWidth()),static_cast<float>(getHeight()), 15,16);
			
			// OSCILATOR PANEL
			
			float panelX = 0.25;
			float panelY = 0;

			GUI->group_visible[0] = true;
		
			int drum_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 14.5,13, "", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, drum_panel, "global", "");
			
			//---------------------------------------------------------------------------------
			
			int drum_group = Delirium_UI_Group_Create(GUI, "drums");
			Delirium_UI_Group_Add_Member(GUI, "drums", "KICK");
			Delirium_UI_Group_Add_Member(GUI, "drums", "SNARE");
			Delirium_UI_Group_Add_Member(GUI, "drums", "CLHAT");
			Delirium_UI_Group_Add_Member(GUI, "drums", "OPHAT");
			Delirium_UI_Group_Add_Member(GUI, "drums", "FX");
			Delirium_UI_Group_Add_Member(GUI, "drums", "PRESETS");
			
			int nav_drumss = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX + 0, panelY + 0.25, 11.5,0.5,"",-1);	
						
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "KICK", "drums", "KICK");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "SNARE", "drums", "SNARE");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "CLOSED HAT", "drums", "CLHAT");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "OPEN HAT", "drums", "OPHAT");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "FX", "drums", "FX");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "PRESETS", "drums", "PRESETS");
			Delirium_UI_Group_Set_Visible_member(GUI, "drums", "KICK");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_drumss, "global", "");
			
			//------------------------------------------------------------------------------------
			// MASTER CONTROLS
			
			int gui_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0,
				panelX + 11.25, panelY + 4.25, 3, 3, "VOLUME", stegosaurus_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_VOLUME, 0,2);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_VOLUME, "global", "");
			fParameters_widget_number[stegosaurus_VOLUME] = gui_VOLUME;

			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - KICK
			
			// OSCILLATOR ONE
			
			int gui_KICK_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_KICK_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_ACTIVE, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC1_ACTIVE] = gui_KICK_OSC1_ACTIVE;
			
			
			int gui_KICK_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_KICK_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_NOISE, 0.25,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_NOISE, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC1_NOISE] = gui_KICK_OSC1_NOISE;
			
			int gui_KICK_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_KICK_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_PITCH, 0.25,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_PITCH, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC1_PITCH] = gui_KICK_OSC1_PITCH;
			
			int gui_KICK_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_KICK_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK] = gui_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+1] = gui_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+2] = gui_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+3] = gui_KICK_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_AMP_ATTACK, "drums", "KICK");

			
			int gui_KICK_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_KICK_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_PITCH_ATTACK] = gui_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_PITCH_ATTACK+1] = gui_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_PITCH_ATTACK+2] = gui_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_PITCH_ATTACK+3] = gui_KICK_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_PITCH_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_KICK_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_VOLUME, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC1_VOLUME] = gui_KICK_OSC1_VOLUME;
			
			// OSCILLATOR TWO
			
			int gui_KICK_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_KICK_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_ACTIVE, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC2_ACTIVE] = gui_KICK_OSC2_ACTIVE;
			
			int gui_KICK_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_KICK_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_NOISE, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC2_NOISE] = gui_KICK_OSC2_NOISE;
			
			int gui_KICK_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_KICK_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_PITCH, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC2_PITCH] = gui_KICK_OSC2_PITCH;
			
			int gui_KICK_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_KICK_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC2_AMP_ATTACK] = gui_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_AMP_ATTACK+1] = gui_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_AMP_ATTACK+2] = gui_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_AMP_ATTACK+3] = gui_KICK_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_AMP_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_KICK_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC2_PITCH_ATTACK] = gui_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_PITCH_ATTACK+1] = gui_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_PITCH_ATTACK+2] = gui_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC2_PITCH_ATTACK+3] = gui_KICK_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_PITCH_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_KICK_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_VOLUME, "drums", "KICK");
			fParameters_widget_number[stegosaurus_KICK_OSC2_VOLUME] = gui_KICK_OSC2_VOLUME;
			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - SNARE
			
			// OSCILLATOR ONE
			
			int gui_SNARE_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_SNARE_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_ACTIVE, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC1_ACTIVE] = gui_SNARE_OSC1_ACTIVE;
			
			int gui_SNARE_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_SNARE_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_NOISE, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC1_NOISE] = gui_SNARE_OSC1_NOISE;
			
			int gui_SNARE_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_SNARE_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_PITCH, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC1_PITCH] = gui_SNARE_OSC1_PITCH;
			
			int gui_SNARE_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_SNARE_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK] = gui_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+1] = gui_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+2] = gui_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+3] = gui_SNARE_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_AMP_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_SNARE_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_PITCH_ATTACK] = gui_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_PITCH_ATTACK+1] = gui_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_PITCH_ATTACK+2] = gui_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_PITCH_ATTACK+3] = gui_SNARE_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_PITCH_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_SNARE_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_VOLUME, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC1_VOLUME] = gui_SNARE_OSC1_VOLUME;
			
			// OSCILLATOR TWO
			
			int gui_SNARE_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_SNARE_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_ACTIVE, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC2_ACTIVE] = gui_SNARE_OSC2_ACTIVE;
			
			int gui_SNARE_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_SNARE_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_NOISE, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC2_NOISE] = gui_SNARE_OSC2_NOISE;
			
			int gui_SNARE_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_SNARE_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_PITCH, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC2_PITCH] = gui_SNARE_OSC2_PITCH;
			
			int gui_SNARE_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_SNARE_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_AMP_ATTACK] = gui_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_AMP_ATTACK+1] = gui_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_AMP_ATTACK+2] = gui_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_AMP_ATTACK+3] = gui_SNARE_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_AMP_ATTACK, "drums", "SNARE");
						
			
			int gui_SNARE_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_SNARE_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_PITCH_ATTACK] = gui_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_PITCH_ATTACK+1] = gui_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_PITCH_ATTACK+2] = gui_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC2_PITCH_ATTACK+3] = gui_SNARE_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_PITCH_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_SNARE_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_VOLUME, "drums", "SNARE");
			fParameters_widget_number[stegosaurus_SNARE_OSC2_VOLUME] = gui_SNARE_OSC2_VOLUME;
				
			//------------------------------------------------------------------------------------
			// DRUMS GUI - CLOSED HAT
			
			// OSCILLATOR ONE
			
			int gui_CLHAT_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_CLHAT_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_ACTIVE, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_ACTIVE] = gui_CLHAT_OSC1_ACTIVE;
			
			int gui_CLHAT_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_CLHAT_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_NOISE, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_NOISE] = gui_CLHAT_OSC1_NOISE;
			
			int gui_CLHAT_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_CLHAT_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_PITCH, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_PITCH] = gui_CLHAT_OSC1_PITCH;
			
			int gui_CLHAT_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_CLHAT_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = gui_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+1] = gui_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+2] = gui_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+3] = gui_CLHAT_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_AMP_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_CLHAT_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_PITCH_ATTACK] = gui_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_PITCH_ATTACK+1] = gui_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_PITCH_ATTACK+2] = gui_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_PITCH_ATTACK+3] = gui_CLHAT_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_PITCH_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_CLHAT_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_VOLUME, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_VOLUME] = gui_CLHAT_OSC1_VOLUME;
			
			// OSCILLATOR TWO
			
			int gui_CLHAT_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_CLHAT_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_ACTIVE, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_ACTIVE] = gui_CLHAT_OSC2_ACTIVE;
			
			int gui_CLHAT_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_CLHAT_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_NOISE, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_NOISE] = gui_CLHAT_OSC2_NOISE;
			
			int gui_CLHAT_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_CLHAT_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_PITCH, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_PITCH] = gui_CLHAT_OSC2_PITCH;
			
			int gui_CLHAT_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_CLHAT_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_AMP_ATTACK] = gui_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_AMP_ATTACK+1] = gui_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_AMP_ATTACK+2] = gui_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_AMP_ATTACK+3] = gui_CLHAT_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_AMP_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_CLHAT_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_PITCH_ATTACK] = gui_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_PITCH_ATTACK+1] = gui_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_PITCH_ATTACK+2] = gui_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_PITCH_ATTACK+3] = gui_CLHAT_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_PITCH_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_CLHAT_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_VOLUME, "drums", "CLHAT");
			fParameters_widget_number[stegosaurus_CLHAT_OSC2_VOLUME] = gui_CLHAT_OSC2_VOLUME;
			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - OPEN HAT
			
			// OSCILLATOR ONE
			
			int gui_OPHAT_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_OPHAT_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_ACTIVE, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_ACTIVE] = gui_OPHAT_OSC1_ACTIVE;
			
			int gui_OPHAT_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_OPHAT_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_NOISE, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_NOISE] = gui_OPHAT_OSC1_NOISE;
			
			int gui_OPHAT_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_OPHAT_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_PITCH, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_PITCH] = gui_OPHAT_OSC1_PITCH;
			
			int gui_OPHAT_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_OPHAT_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = gui_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+1] = gui_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+2] = gui_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+3] = gui_OPHAT_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_AMP_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_OPHAT_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_PITCH_ATTACK] = gui_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_PITCH_ATTACK+1] = gui_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_PITCH_ATTACK+2] = gui_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_PITCH_ATTACK+3] = gui_OPHAT_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_PITCH_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_OPHAT_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_VOLUME, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_VOLUME] = gui_OPHAT_OSC1_VOLUME;
			
			// OSCILLATOR TWO
			
			int gui_OPHAT_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_OPHAT_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_ACTIVE, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_ACTIVE] = gui_OPHAT_OSC2_ACTIVE;
			
			int gui_OPHAT_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_OPHAT_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_NOISE, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_NOISE, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_NOISE] = gui_OPHAT_OSC2_NOISE;
			
			int gui_OPHAT_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_OPHAT_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_PITCH, 2,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_PITCH, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_PITCH] = gui_OPHAT_OSC2_PITCH;
			
			int gui_OPHAT_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_OPHAT_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_AMP_ATTACK] = gui_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_AMP_ATTACK+1] = gui_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_AMP_ATTACK+2] = gui_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_AMP_ATTACK+3] = gui_OPHAT_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_AMP_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_OPHAT_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_PITCH_ATTACK] = gui_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_PITCH_ATTACK+1] = gui_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_PITCH_ATTACK+2] = gui_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_PITCH_ATTACK+3] = gui_OPHAT_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_PITCH_ATTACK, "drums", "OPHAT");
			
			
			int gui_OPHAT_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_OPHAT_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_VOLUME, "drums", "OPHAT");
			fParameters_widget_number[stegosaurus_OPHAT_OSC2_VOLUME] = gui_OPHAT_OSC2_VOLUME;
				
			//--------------------------------------------------------------------------------------
			
			// PRESETS

			widget_categories_list = Delirium_UI_Create_Widget(GUI, deliriumUI_List, 0, panelX + 0.25,panelY + 1.25, 5,5.9, "CATEGORIES", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_categories_list, "drums", "PRESETS");
					
			widget_presets_list = Delirium_UI_Create_Widget(GUI, deliriumUI_List, 0, panelX + 5.75,panelY + 1.25, 8.5, 5.9, "PRESETS", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_presets_list, "drums", "PRESETS");			
			
			//--------------------------------------------------------------------------------------
			
			
			loadSymbols();
			searchPresets();

			GUI->draw_flag = true;					
			GUI->drag = 0;
			GUI->drawn_at_least_once = 0;
			GUI->current_widget = -1;	
			
			Delirium_UI_Group_Set_Active_Widgets(GUI);

		}
		
		//------------------------------------------------------------------------------------------------------

		vector<string> split (const string &s, char delim) {
			    vector<string> result;
			    stringstream ss (s);
			    string item;
	
			    while (getline (ss, item, delim)) {
			        result.push_back (item);
				    }

		    return result;
		}
		
		//------------------------------------------------------------------------------------------------------
		void loadSymbols()
		{
			string lv2_path = "/usr/lib/lv2"; // getenv("LV2_PATH");
			string line;
			struct dirent *d;
			struct stat st;
			DIR *dr;
			stringstream stegosaurus_ttl_file_name;
			number_of_symbols = 0;
					
			for (int x=0; x<200; x++) symbols.push_back("");
			
			vector<string> v = split (lv2_path, ':');
			
			for (int z=0; z<v.size(); z++)
			{
				stegosaurus_ttl_file_name.str("");
				stegosaurus_ttl_file_name << v[z] << "/stegosaurus.lv2/stegosaurus_dsp.ttl";


				ifstream stegosaurus_ttl_file(stegosaurus_ttl_file_name.str().c_str(), ios::in);
				
				while (getline(stegosaurus_ttl_file,line))  
				{
				
					stringstream symbol_name;
					symbol_name.str("");
					int symbol_index;
					bool symbol_found = false;
						
					int search_pos = line.rfind("lv2:symbol");
					if (search_pos > 0)
					{
						symbol_found = true;
						int add_this_char = 0;
						
						for (int char_number = 0; char_number < line.length(); char_number++)
						{
							if (line[char_number] == 34) add_this_char = 1 - add_this_char;
							else if (add_this_char) symbol_name << line[char_number];
						}
						
						

					}					
					
					search_pos = line.rfind("lv2:index");
					
					if (search_pos > 0)
					{
						istringstream (line.substr(search_pos+10)) >> symbol_index;
					}
					
					if (symbol_found)
					{
						symbols[symbol_index] = symbol_name.str();
						if (symbol_index > number_of_symbols) number_of_symbols = symbol_index;
						
					}	
				}
				
				stegosaurus_ttl_file.close();
			}
		}

		

		//------------------------------------------------------------------------------------------------------
		
		void searchPresets()
		{
			string lv2_path = "/usr/lib/lv2"; // getenv("LV2_PATH");
			stringstream ss;
			struct dirent *d, *pr_d;
			struct stat st;
			DIR *dr, *prDr;
			stringstream file_name_and_path;
			stringstream path_name;
			

			vector<string> v = split (lv2_path, ':');
			    
			for (int z=0; z<v.size(); z++)
			{
				dr = opendir(v[z].c_str()); // search through LV2 folders in LV2_PATH 
				
				if (dr!=NULL)
				{
					for( d=readdir(dr); d!=NULL; d=readdir(dr)) // List all files here
					{
						string file_name = d->d_name;
							if (file_name == "stegosaurus.lv2")
							{
								file_name = "stegosaurus.lv2/stegosaurus_presets.lv2";
							}
							
							if (stat(file_name.c_str(),&st)) // Look in each folder
							{
								path_name.str("");
								path_name << v[z] << "/" << file_name;
								prDr = opendir(path_name.str().c_str()); // Read file see if it applies to our plugin

								if (prDr!=NULL)
								{
									for( pr_d=readdir(prDr); pr_d!=NULL; pr_d=readdir(prDr))
									{
										file_name_and_path.str("");
										file_name_and_path <<  v[z] << "/" << file_name << "/" << pr_d->d_name;
										
										int file_is_ttl = file_name_and_path.str().rfind(".ttl");
										int file_is_manifest = file_name_and_path.str().rfind("manifest.ttl");
										int file_is_stegosaurus = file_name_and_path.str().rfind("stegosaurus.ttl");
										
										if (file_is_ttl > 0 && file_is_manifest < 0 && file_is_stegosaurus < 0)
										{
																
											bool is_stegosaurus_preset = false;
											ifstream preset_file(file_name_and_path.str().c_str(), ios::in);
											string line;
									
											while (getline(preset_file,line))  
											{
												int search_pos = line.rfind("stegosaurus");
												if (search_pos > 0)
												{
													is_stegosaurus_preset = true;

												}
										
											}
																			
											preset_file.close();
											if (is_stegosaurus_preset)
											{
											
												preset new_preset;
												new_preset.file = file_name_and_path.str().c_str();
												string preset_name = pr_d->d_name;
												new_preset.name = preset_name.substr(0,preset_name.length()-4);
												
												string category_name = Find_Preset_Category(new_preset.file);
												bool category_found = false;
												
												for (int x=0; x<categories.size(); x++)
												{
													if (categories[x].name == category_name)
													{
														categories[x].presets.push_back(new_preset);
														category_found = true;
													}
												}
												
												if (!category_found)
												{
													category new_category;
													new_category.name = category_name;
													new_category.presets.push_back(new_preset);	
																				categories.push_back(new_category);																																					
												}
												// Find_Preset_Category(new_preset.file);
											}	
										}			
								}
								closedir(prDr);
							}
						}
					}
					closedir(dr);
				}				
			}		
			
			sort(categories.begin(),categories.end(),alphasort_category());
			
			for (int x=0; x<categories.size(); x++)
			{
				Delirium_UI_Widget_List_Add_Item(GUI, widget_categories_list, categories[x].name);
			}
			
			for (int x=0; x<categories.size(); x++)
			{
				sort(categories[x].presets.begin(),categories[x].presets.end(),alphasort_preset());	
			}
			
		}
		
		//--------------------------------------------------------------------------------------
		// GET SYMBOL INDEX NUMBER (PORT)
		
		int getSymbolIndex(string symbol_name)
		{
			int symbol_index = -1;
			
			for (int x=0; x<number_of_symbols; x++)
			{
				if (symbols[x] == symbol_name)
					symbol_index = x;
			}	
			return symbol_index;
		}
		
		//--------------------------------------------------------------------------------------
		// LOAD PRESET

		void loadPreset()
		{ 	
			int category_number = current_category;

			int preset_number = GUI->Widgets[widget_presets_list]->list_position
				+ GUI->Widgets[widget_presets_list]->list_scroll;
				
			if ( preset_number > categories[category_number].presets.size()-1
				|| category_number > categories.size()-1
				|| categories[category_number].presets.size() == 0) return;
				
			string preset_path_and_file_name = categories[category_number].presets[preset_number].file;			

			ifstream preset_file;
	
			preset_file.open(preset_path_and_file_name );

			string preset_symbol;

			string temp_str;
			string line;
			double preset_value;
			string symbol_name = "";
			
			while (getline(preset_file,line))  
			{
				int preset_index = line.rfind("lv2:symbol ");
				if (preset_index > 0) preset_symbol = line.substr(preset_index + 12, line.length() - preset_index - 15);

				preset_index = line.rfind("pset:value");
				if (preset_index > 0) istringstream ( line.substr(preset_index + 11, line.length()+1 ) ) >> preset_value;

				if (preset_index > 0)
				{
				
					int symbol_index = getSymbolIndex(preset_symbol) - 5;
					
					if (symbol_index > -1) 
					{ 
						parameterChanged(symbol_index, preset_value); 
					}

				} 
			}
			
		
						
			preset_file.close();

		}

		//------------------------------------------------------------------------------------------------------
		string Find_Preset_Category(string file_name)
		{
		
			ifstream preset_file;
			preset_file.open(file_name );

			
			string category_name = "Unsorted";
			string line;
			
			while (getline(preset_file,line))  
			{

				int preset_index = line.rfind("pset:bank");

				if (preset_index > 0)
				{
				
					int start_index = line.rfind("<");
					int end_index = line.rfind(">") - start_index - 1;
					category_name =  line.substr(start_index+1, end_index);
					

				} 
			}
			
		
						
			preset_file.close();
		
			return category_name;
		}







	protected:
	
		//------------------------------------------------------------------------------------------------------

   		 void onCairoDisplay(const CairoGraphicsContext& context)
    		{
			cr = context.handle;
						
			Delirium_UI_Display_All(GUI, cr);
			

		}
		
		//------------------------------------------------------------------------------------------------------
		
		void parameterChanged(uint32_t index, float value) override
		{


			// get widget number connected to this parameter
			int widget_number = fParameters_widget_number[index];
			
			if (index == stegosaurus_SNARE_OSC2_PITCH_DECAY)
			{
				cout << index << " - " << value << " - " << widget_number <<  " - " << GUI->Widgets.size() << endl;

			}
						
			if (widget_number < 0 || widget_number > GUI->Widgets.size() ) return;
			


			Delirium_UI_Widget_Base* wdg = (Delirium_UI_Widget_Base*)GUI->Widgets[widget_number];
			

			
			if (index == wdg->parameter_number && wdg->type == deliriumUI_ADSR) wdg->current_value = 0;
			
			if (index > wdg->parameter_number && wdg->type == deliriumUI_ADSR)
			{
				wdg->current_value = ( index - wdg->parameter_number);
				if (index == stegosaurus_SNARE_OSC2_PITCH_DECAY) cout << "grrrrrrrr - " << index << " - " << wdg->parameter_number << endl;
			}
			
			if ((int)index > wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
				wdg->route_number = value;

			if ((int)index == wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
				Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
	
			
			if (wdg->type != deliriumUI_Fader_Route) Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
			
			// trigger repaint

			fParameters[index] = value;
			setParameterValue(index, value);
			

			
			GUI->draw_flag = true;		
			repaint();
			
		}

		//--------------------------------------------------------------------------------------------------------
		void programLoaded(uint32_t index) override
		{
			GUI->draw_flag = true;
			Delirium_UI_Display_All(GUI, cr);
		}
		
		//------------------------------------------------------------------------------------------------------

		bool onMouse(const MouseEvent& ev) override
		{
			// cairo_t* cr = getParentWindow().getGraphicsContext().cairo;	
			
			
			if (ev.button == 1)
			{
				Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
				Delirium_UI_Left_Button_Press(GUI, cr, ev.pos.getX(), ev.pos.getY());
				repaint();
				
				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				
								
				if ( GUI->current_widget == widget_categories_list)
				{
					int category_number = GUI->Widgets[widget_categories_list]->list_position
						+ GUI->Widgets[widget_categories_list]->list_scroll;
				
					GUI->Widgets[widget_presets_list]->list_items.clear();
					GUI->Widgets[widget_presets_list]->list_scroll = 0;
					current_category = category_number;

					for (int pr=0; pr<categories[category_number].presets.size(); pr++)
					{			
						Delirium_UI_Widget_List_Add_Item(GUI, widget_presets_list, categories[category_number].presets[pr].name);
					}
					
				}
				
				if ( GUI->current_widget == widget_presets_list )
				{
					loadPreset();
				}
			
				if (parameter_number > -1)
				{	
				
					float value = Delirium_UI_Widget_Get_Value(GUI);
					setParameterValue(parameter_number, value);
			

				}
				return true;

			}


			if (ev.button == 2)
			{

				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > -1)
				{
	
					setParameterValue(parameter_number, 0);
					Delirium_UI_Middle_Button_Press(GUI);	
					repaint();	
					
			
				}
			}

			return true;
		}

		//------------------------------------------------------------------------------------------------------

		bool onMotion(const MotionEvent& ev)
		{
			Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());

			repaint();	

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
				
				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
	 
			}
			return true;

		}
		
		
		//------------------------------------------------------------------------------------------------------

		bool onScroll(const ScrollEvent& ev)
		{
			float delta = ev.delta.getY();
			// cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

			int current_widget = GUI->current_widget;

			if (current_widget > 0)
			{	
				if (GUI->Widgets[current_widget]->type == deliriumUI_Panel) return true;
			
				GUI->Widgets[current_widget]->Mouse_Scroll(ev.pos.getX(), ev.pos.getY(), delta);
				Delirium_UI_Convert_Value_To_Range(GUI, current_widget);
				GUI->Widgets[current_widget]->Draw(cr);
				repaint();
			}

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);

				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
				

			}

			return true;
		}

		
		
		
	private:
		cairo_t* cr;
		Delirium_UI_Surface* GUI;
		float fParameters[kParameterCount+10];
		int fParameters_widget_number[kParameterCount+10];
		
		vector <string> symbols;
		int number_of_symbols;
		
		struct preset
		{
			string name;
			string file;
		};
		
		struct category
		{
			string name;
			vector <preset> presets;
		};
		
		struct alphasort_preset
		{
			inline bool operator() (const preset& struct1, const preset& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};

		vector <category> categories;
		
		int widget_categories_list;
		int widget_presets_list;
		int current_category;
		
		struct alphasort_category
		{
			inline bool operator() (const category& struct1, const category& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};


};

UI* createUI()
{
    return new stegosaurus_ui;
}

END_NAMESPACE_DISTRHO


