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
int power = 100;
int counter = 0;
int pid_state = 0;
double prev_temp = 100;


void set_switch(int state){
	if (state == 1){
		portOut(1, 1 << 7);
		counter = 0;
	}else{
		portOut(1, 0);
		counter = 0;
	}
}


double prev_temps[5];
int number = 0;


void onoffctrl(int panel){
	if (pid_state){
		double req_temp;
		double temp;
		GetCtrlVal(panel, PANEL_NUMERIC, &req_temp);
		analogIn(2, &temp);
		temp *= 100;
		double integ = 0;
		//ouble diff = (temp - prev_temp)/0.2;
		double k1 = 1.0;
		double k2 = 5.0;
		double k3 = 10.0;
		
		if (number == 5){
			number = 0;
		}
		prev_temps[number] = temp;
		
		double diff[5];
		
		for(int i=0; i < 5; i++){
			diff[i]= req_temp - prev_temps[i];
			integ += diff[i] * 0.1;
		}
		
		power = k1 * (req_temp - temp) + k2 * integ; // + k2 * integ + k3 * diff);
		//if ()
		SetCtrlVal(panelHandle,PANEL_NUMERIC_2, power);
		
		//prev_temp = temp;
		
		
		/*
		if ((temp - req_temp)/2 > 0.05){
			set_switch(0);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 0);
			power -= 1;
			SetCtrlVal(panelHandle,PANEL_NUMERIC_2, power);
		}
		if(temp < req_temp){
			set_switch(1);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 1);
		}else{
			if(temp > req_temp){
			set_switch(0);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 0);
			} 
		
		}*/
	}
}


void onoffctrlxxx(int panel){
	if (pid_state){
		double req_temp;
		double temp;
		GetCtrlVal(panel, PANEL_NUMERIC, &req_temp);
		analogIn(2, &temp);
		temp *= 100;
		
		double delta = temp - req_temp;
		
		if (delta < 0){
			delta *= -1;
		}
		
		if (((temp - req_temp)/2 > 0.05) && ((temp - req_temp)/2 < 1.)){	 //
			set_switch(0);
			power = (int)(28000 * delta);
			SetCtrlVal(panel,PANEL_NUMERIC_2, power);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 0);
		}
		if(temp < req_temp){
			set_switch(1);
			power = (int)(300 * delta);
			SetCtrlVal(panel,PANEL_NUMERIC_2, power);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 1);
		}else{
			if(temp > req_temp){
			set_switch(0);
			SetCtrlVal(panel,PANEL_NUMERIC_2, 0) ;
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, 0);
			} 
		
		}
	}
}


int CVICALLBACK cbtimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			
			double volt;
			analogIn(2, &volt);
			
			//SetCtrlVal(panelHandle,PANEL_TEMP, volt * 100);
			SetCtrlVal(panelHandle,PANEL_NUMERICTHERM, volt*100);
			PlotStripChartPoint(panel, PANEL_STRIPCHART, volt*100);
			//GetCtrlVal(panel, PANEL_NUMERIC_2, &power);
			onoffctrl(panel);
			
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

int CVICALLBACK BTNcb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:

			int state;
			
			GetCtrlVal(panel, PANEL_TOGGLEBUTTON, &state);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, state);
			GetCtrlVal(panel, PANEL_NUMERIC_2, &power);
			if (state == 1){
				double temp;
				GetCtrlVal(panel,PANEL_NUMERICTHERM, &temp);
				for(int i = 0; i++; i<5){
					prev_temps[i] = temp; 
				}
			}
			
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

int CVICALLBACK controlcb (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			
			int state;
			GetCtrlVal(panel, PANEL_CONTROLBUTTON, &state);
			pid_state = state;
			set_switch(state);
			SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, state);
			GetCtrlVal(panel, PANEL_NUMERICTHERM, &prev_temp);
			//SetCtrlAttribute(panel, PANEL_CONTROLTIMER, ATTR_ENABLED, state);
			//shimctrl(panel, state);

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

int CVICALLBACK timecontrol (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			
			if (counter > power){
				set_switch(0);
				counter += 1;
				if (counter == 100){
					counter = 0;
				}
			}else{
				if (counter == 0){
					set_switch(1);
				}
				counter += 1;
			}
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
