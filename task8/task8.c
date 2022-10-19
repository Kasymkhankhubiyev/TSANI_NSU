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
//double prev_temp = 100;


void set_switch(int state){
	if (state == 1){
		portOut(1, 1 << 7);
		counter = 0;
	}else{
		portOut(1, 0);
		counter = 0;
	}
}

#define n 100

double prev_temps[n];
int number = 0;


void onoffctrl(int panel){
	if (pid_state == 1){
		double req_temp;
		double temp;
		GetCtrlVal(panel, PANEL_NUMERIC, &req_temp);
		analogIn(2, &temp);
		temp *= 100;
		//temp += 10;
		double integ = 0;
		double diffitem = 0;
		//ouble diff = (temp - prev_temp)/0.2;
		double k1 = 0;
		double k2 = 0;
		double k3 = 0;
		double diff[n];
		
		GetCtrlVal(panel, PANEL_K1, &k1);
		GetCtrlVal(panel, PANEL_K2, &k2);
		GetCtrlVal(panel, PANEL_K3, &k3);
		
		prev_temps[number] = temp;
		number+=1;
		if (number == n){
			number = 0;
		}
		
		
		for(int i=0; i < n; i++){
			diff[i]= req_temp - prev_temps[i];
		}
		
		integ = 0;
		for(int i = 1; i < n; i++){
			integ += diff[i];
		}
		
		double diff1 = 0;
		for(int i = 1; i <n; i++){
			diff1 += (diff[i] - diff[i-1])/0.1;
		}
		
		double diff2 = 0;
		for(int i = 50; i<n; i++){
			diff2 += (diff[i] - diff[i-1])/0.1;
		}
		
		diffitem = (diff2 - diff1)/50.;
		
		power = k1 * (req_temp - temp) + k2 * integ + k3 * diffitem;
				
		SetCtrlVal(panelHandle,PANEL_K1val, k1 * (req_temp - temp));
		SetCtrlVal(panelHandle,PANEL_K2val, k2 * integ);
		SetCtrlVal(panelHandle,PANEL_K3val, k3 * diffitem);
				
		SetAxisScalingMode(panel, PANEL_STRIPCHART_3,VAL_LEFT_YAXIS, VAL_AUTOSCALE, -800.0, 800.0);
		PlotStripChartPoint(panel, PANEL_STRIPCHART_3, k1 * (req_temp - temp));
				
		SetAxisScalingMode(panel, PANEL_STRIPCHART_4,VAL_LEFT_YAXIS, VAL_AUTOSCALE, -800.0, 800.0);
		PlotStripChartPoint(panel, PANEL_STRIPCHART_4, k2 * integ);
				
		SetAxisScalingMode(panel, PANEL_STRIPCHART_5,VAL_LEFT_YAXIS, VAL_AUTOSCALE, -800.0, 800.0);
		PlotStripChartPoint(panel, PANEL_STRIPCHART_5, k3 * diffitem);
		
		if((temp - req_temp) < -10.) power = 100;
		if((temp - req_temp) > 10.) power = 0;
		if (power < 0){
				power = 0;
		}
		if (power > 100){
				power = 100;
		}
		SetCtrlVal(panelHandle,PANEL_NUMERIC_2, power);   	
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
			
			SetCtrlVal(panelHandle,PANEL_NUMERICTHERM, volt*100);
			PlotStripChartPoint(panel, PANEL_STRIPCHART, volt*100);
			onoffctrl(panel);
			
			double req_temp;
			
			GetCtrlVal(panel, PANEL_NUMERIC, &req_temp);
			
			
			double dif; 
			dif = req_temp - volt*100;
			
			
			SetAxisScalingMode(panel, PANEL_STRIPCHART_2,VAL_LEFT_YAXIS, VAL_AUTOSCALE, -80.0, 80.0);
			PlotStripChartPoint(panel, PANEL_STRIPCHART_2, dif);
			
			unsigned char data;
			portIn(1, &data);
			
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
			if (state == 1){
				double temp;
				GetCtrlVal(panel,PANEL_NUMERICTHERM, &temp);
				for(int i = 0; i<n; i++){
					prev_temps[i] = temp; 
				}
			}
			

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
			
			if (counter >= power){
				set_switch(0);
				
			}
			else{
				set_switch(1);
			}
			
			counter += 1;
			if (counter >= 100){
				counter = 0;
			}
			
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}
