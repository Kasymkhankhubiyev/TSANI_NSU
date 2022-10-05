//==============================================================================
//
// Title:       task8
// Purpose:     A short description of the application.
//
// Created on:  05.10.2022 at 15:30:52 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "task8.h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "task8.uir", PANEL));
	
	ni6251Slot(2);
	int mask = 0x80; //10000000
	portMask(1, mask);
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int heatst = 0;
int overheatst = 0;

int CVICALLBACK cbtimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			
			double volt;
			analogIn(2, &volt);
			
			//SetCtrlVal(panelHandle,PANEL_TEMP, volt * 100);
			SetCtrlVal(panelHandle,PANEL_NUMERICTHERM, volt*100);
			PlotStripChartPoint(panel, PANEL_STRIPCHART, volt*100);
			
			unsigned char data;
			portIn(1, &data);
			
			//int heat = data & (1 << 7);
			int overheat = data & (1 << 6);
			
			
			if ((!overheat) && (1<<6)){
				SetCtrlVal(panel, PANEL_LED, 1);
				overheatst = 1;
			}
			else{
				if (overheatst == 1){
					SetCtrlVal(panel, PANEL_LED, 0);
				}
			}

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK cbtermometr (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:

			break;
		case EVENT_LEFT_CLICK:

			break;
		case EVENT_RIGHT_CLICK:

			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

void set_switch(int state){
	if (state == 1){
		portOut(1, 1 << 7);
	}else{
		portOut(1, 0);
	}
}

int percent = 0;

int CVICALLBACK BTNcb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:

			int state;
			
			GetCtrlVal(panel, PANEL_TOGGLEBUTTON, &state);
			SetCtrlAttribute(panel, PANEL_STIMER, ATTR_ENABLED, state);
			GetCtrlVal(panel, PANEL_NUMERIC_2, &percent);
			
			set_switch(state);
			
			
		case EVENT_LEFT_CLICK:

			break;
		case EVENT_RIGHT_CLICK:

			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
int on = 1;
int sign = 0;
int bigtimer = 0;

int counter = 0;

void shimctrl(int panel, int state){
	
	//SetCtrlAttribute(panel, PANEL_TIMER_2, ATTR_ENABLED, state);
	//if (state == 0){
		//SetCtrlAttribute(panel, PANEL_STIMER, ATTR_ENABLED, state);
	//}
}

int CVICALLBACK controlcb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			
			int state;
			GetCtrlVal(panel, PANEL_CONTROLBUTTON, &state);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, state);
			shimctrl(panel, state);

			break;
		case EVENT_LEFT_CLICK:

			break;
		case EVENT_RIGHT_CLICK:

			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

void onoffctrl(int panel){
	double req_temp;
	double temp;
	GetCtrlVal(panel, PANEL_NUMERIC, &req_temp);
	analogIn(2, &temp);
	temp *= 100;
	if (abs(temp - req_temp)/2 > 0.1){
		set_switch(0);
	}
	if(temp < req_temp){
		set_switch(1);
	}else{
		if(temp > req_temp){
			set_switch(0);
		} 
		
	}
}

int CVICALLBACK timecontrol (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			
			onoffctrl(panel);
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}


int CVICALLBACK stimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			if (counter == percent/10){
				set_switch(0);
				counter = 0;
			}else{
				counter += 1;
			}
			//if (on){
				//set_switch(0);
				//on = 0;
			//}else{
				//set_switch(1);
				//on = 1;
			//}
			

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK timebig (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			if (bigtimer == 1){
				SetCtrlAttribute(panel, PANEL_STIMER, ATTR_ENABLED, 0);
				bigtimer = 0;
			}else{
				SetCtrlAttribute(panel, PANEL_STIMER, ATTR_ENABLED, 1);
				bigtimer = 1;
			}

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
