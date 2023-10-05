

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
			
			int nav_drumss = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX + 0, panelY + 0.25, 11.5,0.5,"",-1);	
						
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "KICK", "drums", "KICK");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "SNARE", "drums", "SNARE");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "CLOSED HAT", "drums", "CLHAT");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "OPEN HAT", "drums", "OPHAT");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_drumss, "FX", "drums", "FX");
			Delirium_UI_Group_Set_Visible_member(GUI, "drums", "KICK");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_drumss, "global", "");
			
			//------------------------------------------------------------------------------------
			// MASTER CONTROLS
			
			int gui_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0,
				panelX + 11.25, panelY + 4.25, 3, 3, "VOLUME", stegosaurus_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_VOLUME, 0,2);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_VOLUME, "global", "");
			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - KICK
			
			// OSCILLATOR ONE
			
			int gui_KICK_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_KICK_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_ACTIVE, "drums", "KICK");
			
			int gui_KICK_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_KICK_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_NOISE, "drums", "KICK");
			
			int gui_KICK_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_KICK_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_PITCH, "drums", "KICK");
			
			int gui_KICK_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_KICK_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK] = stegosaurus_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+1] = stegosaurus_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+2] = stegosaurus_KICK_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+3] = stegosaurus_KICK_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_AMP_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_KICK_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK] = stegosaurus_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+1] = stegosaurus_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+2] = stegosaurus_KICK_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+3] = stegosaurus_KICK_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_PITCH_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_KICK_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC1_VOLUME, "drums", "KICK");
			
			// OSCILLATOR TWO
			
			int gui_KICK_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_KICK_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_ACTIVE, "drums", "KICK");
			
			int gui_KICK_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_KICK_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_NOISE, "drums", "KICK");
			
			int gui_KICK_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_KICK_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_PITCH, "drums", "KICK");
			
			int gui_KICK_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_KICK_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK] = stegosaurus_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+1] = stegosaurus_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+2] = stegosaurus_KICK_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+3] = stegosaurus_KICK_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_AMP_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_KICK_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK] = stegosaurus_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+1] = stegosaurus_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+2] = stegosaurus_KICK_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_KICK_OSC1_AMP_ATTACK+3] = stegosaurus_KICK_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_PITCH_ATTACK, "drums", "KICK");
			
			int gui_KICK_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_KICK_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_KICK_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_KICK_OSC2_VOLUME, "drums", "KICK");
			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - SNARE
			
			// OSCILLATOR ONE
			
			int gui_SNARE_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_SNARE_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_ACTIVE, "drums", "SNARE");
			
			int gui_SNARE_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_SNARE_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_NOISE, "drums", "SNARE");
			
			int gui_SNARE_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_SNARE_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_PITCH, "drums", "SNARE");
			
			int gui_SNARE_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_SNARE_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK] = stegosaurus_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+1] = stegosaurus_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+2] = stegosaurus_SNARE_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+3] = stegosaurus_SNARE_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_AMP_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_SNARE_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK] = stegosaurus_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+1] = stegosaurus_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+2] = stegosaurus_SNARE_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+3] = stegosaurus_SNARE_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_PITCH_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_SNARE_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC1_VOLUME, "drums", "SNARE");
			
			// OSCILLATOR TWO
			
			int gui_SNARE_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_SNARE_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_ACTIVE, "drums", "SNARE");
			
			int gui_SNARE_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_SNARE_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_NOISE, "drums", "SNARE");
			
			int gui_SNARE_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_SNARE_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_PITCH, "drums", "SNARE");
			
			int gui_SNARE_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_SNARE_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK] = stegosaurus_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+1] = stegosaurus_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+2] = stegosaurus_SNARE_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+3] = stegosaurus_SNARE_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_AMP_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_SNARE_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK] = stegosaurus_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+1] = stegosaurus_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+2] = stegosaurus_SNARE_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_SNARE_OSC1_AMP_ATTACK+3] = stegosaurus_SNARE_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_PITCH_ATTACK, "drums", "SNARE");
			
			int gui_SNARE_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_SNARE_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_SNARE_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_SNARE_OSC2_VOLUME, "drums", "SNARE");
				
				
			//------------------------------------------------------------------------------------
			// DRUMS GUI - CLOSED HAT
			
			// OSCILLATOR ONE
			
			int gui_CLHAT_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_CLHAT_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_ACTIVE, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_CLHAT_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_NOISE, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_CLHAT_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_PITCH, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_CLHAT_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = stegosaurus_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+1] = stegosaurus_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+2] = stegosaurus_CLHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+3] = stegosaurus_CLHAT_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_AMP_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_CLHAT_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = stegosaurus_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+1] = stegosaurus_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+2] = stegosaurus_CLHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+3] = stegosaurus_CLHAT_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_PITCH_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_CLHAT_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC1_VOLUME, "drums", "CLHAT");
			
			// OSCILLATOR TWO
			
			int gui_CLHAT_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_CLHAT_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_ACTIVE, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_CLHAT_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_NOISE, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_CLHAT_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_PITCH, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_CLHAT_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = stegosaurus_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+1] = stegosaurus_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+2] = stegosaurus_CLHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+3] = stegosaurus_CLHAT_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_AMP_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_CLHAT_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK] = stegosaurus_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+1] = stegosaurus_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+2] = stegosaurus_CLHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_CLHAT_OSC1_AMP_ATTACK+3] = stegosaurus_CLHAT_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_PITCH_ATTACK, "drums", "CLHAT");
			
			int gui_CLHAT_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_CLHAT_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_CLHAT_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_CLHAT_OSC2_VOLUME, "drums", "CLHAT");
			
			//------------------------------------------------------------------------------------
			// DRUMS GUI - OPEN HAT
			
			// OSCILLATOR ONE
			
			int gui_OPHAT_OSC1_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 1.5, 1, 1.5, "OSC-1", stegosaurus_OPHAT_OSC1_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_ACTIVE, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 1, 0.5, 3, "NOISE", stegosaurus_OPHAT_OSC1_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_NOISE, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 1, 0.5, 3, "PITCH", stegosaurus_OPHAT_OSC1_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_PITCH, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 1.5, 3, 2,"AMP",stegosaurus_OPHAT_OSC1_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = stegosaurus_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+1] = stegosaurus_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+2] = stegosaurus_OPHAT_OSC1_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+3] = stegosaurus_OPHAT_OSC1_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_AMP_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 1.5, 3, 2,"PITCH",stegosaurus_OPHAT_OSC1_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = stegosaurus_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+1] = stegosaurus_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+2] = stegosaurus_OPHAT_OSC1_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+3] = stegosaurus_OPHAT_OSC1_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_PITCH_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC1_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 1, 0.5, 3, "VOL", stegosaurus_OPHAT_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC1_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC1_VOLUME, "drums", "OPHAT");
			
			// OSCILLATOR TWO
			
			int gui_OPHAT_OSC2_ACTIVE = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0,
				panelX + 0.25, panelY + 4.75, 1, 1.5, "OSC-2", stegosaurus_OPHAT_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_ACTIVE, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_NOISE = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 1.5, panelY + 4.25, 0.5, 3, "NOISE", stegosaurus_OPHAT_OSC2_NOISE);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_NOISE, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_NOISE, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_PITCH = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 2.5, panelY + 4.25, 0.5, 3, "PITCH", stegosaurus_OPHAT_OSC2_PITCH);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_PITCH, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_PITCH, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_AMP_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 3.5, panelY + 4.75, 3, 2,"AMP",stegosaurus_OPHAT_OSC2_AMP_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = stegosaurus_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+1] = stegosaurus_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+2] = stegosaurus_OPHAT_OSC2_AMP_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+3] = stegosaurus_OPHAT_OSC2_AMP_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_AMP_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_PITCH_ATTACK = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,
				panelX + 7, panelY + 4.75, 3, 2,"PITCH",stegosaurus_OPHAT_OSC2_PITCH_ATTACK); 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK] = stegosaurus_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+1] = stegosaurus_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+2] = stegosaurus_OPHAT_OSC2_PITCH_ATTACK; 
			fParameters_widget_number[stegosaurus_OPHAT_OSC1_AMP_ATTACK+3] = stegosaurus_OPHAT_OSC2_PITCH_ATTACK; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_PITCH_ATTACK, "drums", "OPHAT");
			
			int gui_OPHAT_OSC2_VOLUME = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 10.5, panelY + 4.25, 0.5, 3, "VOL", stegosaurus_OPHAT_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, gui_OPHAT_OSC2_VOLUME, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, gui_OPHAT_OSC2_VOLUME, "drums", "OPHAT");
				
			//-----------------------------

			GUI->draw_flag = true;					
			GUI->drag = 0;
			GUI->drawn_at_least_once = 0;
			GUI->current_widget = -1;	
			
			Delirium_UI_Group_Set_Active_Widgets(GUI);

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
			for (int x=0; x<GUI->Widgets.size(); x++)
			{
				if (GUI->Widgets[x]->parameter_number == index)
					Delirium_UI_Widget_Set_Value(GUI, x, value);	
			}		
			
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
		float fParameters[kParameterCount+3];
		int fParameters_widget_number[kParameterCount+3];
		
		vector <string> symbols;
		int number_of_symbols;


};

UI* createUI()
{
    return new stegosaurus_ui;
}

END_NAMESPACE_DISTRHO


