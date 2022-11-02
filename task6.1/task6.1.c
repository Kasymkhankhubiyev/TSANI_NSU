//==============================================================================
//
// Title:       task6.1
// Purpose:     A short description of the application.
//
// Created on:  02.11.2022 at 17:52:27 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include <math.h>
#include "task6.1.h"
#include "toolbox.h"

//==============================================================================
// Constants
double frequency = 1000.0, amplitude = 1.0;
int points_number = 1000;

int wave_form = 0;  
/*
0 - ?????????????? - ?????????
1 - ?????????????
2 - ???????????
3 - ????????????
*/

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Global functions

static int panelHandle;

void InitDevices(void){

	ViStatus gen_state, scope_state;
	char error[22] = "cannot connect slots";
	
	gen_state = fgenSlot(6);  //PXI-5402 - gen
	scope_state = scopeSlot(7); //PXI-5114 - scope
	
	if ((gen_state ==VI_FALSE) || (scope_state == VI_FALSE)){
		SetCtrlVal(panelHandle, PANEL_ERRORS, error); 
	}else SetCtrlVal(panelHandle, PANEL_ERRORS, "Success");	
}

void StopGeneration(){
	
	fgenStop();
}

void CloseDevices(){
	
	fgenClose();
	scopeClose();
}


void StartGeneration(){
	
	switch(wave_form){
		case 0:
			fgenStartStandartWaveForm(amplitude, frequency, FGEN_SINE);
			break;
		case 1:
			fgenStartStandartWaveForm(amplitude, frequency, FGEN_SQUARE);
			break;
		case 2:
			fgenStartStandartWaveForm(amplitude, frequency, FGEN_TRIANGLE);
			break;
		case 3:
			fgenStartStandartWaveForm(amplitude, frequency, FGEN_RAMP_UP);
			break;
	}
	
	scopeVertical("0",3 * amplitude, SCOPE_1_MEG_OHM);
	
	scopeFrequency("", 10 * frequency, points_number);
	
	scopeTrigger("TRIG", amplitude/2, SCOPE_POSITIVE);
}

void ReadPlotWaveForm(){
	
	double* wfm;
	wfm = (double*)malloc(points_number*sizeof(double)); 
	
	scopeStart(points_number);
	
	int nsr;
	double h = 1. / (10. * frequency);
	
	double pointsx[points_number];
	
	for (int i = 0; i < points_number; i++){
		pointsx[i] = h * i;
	}
	
	while(SCOPE_COMPLETE != scopeStatus()){}
		
	scopeFetch("0", wfm, points_number, &nsr);
	
	DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
	SetAxisScalingMode(panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -1.5 * amplitude, 1.5 * amplitude);
	PlotXY(panelHandle, PANEL_GRAPH, pointsx, wfm, points_number, VAL_DOUBLE, VAL_DOUBLE, VAL_FAT_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
	
	
	double imageX[points_number];
	
	FFT(wfm, imageX, points_number);
	
	double results[points_number];
	
	for (int i = 0; i < points_number; i++){
		results[i] = pow(pow(wfm[i], 2.0) + pow(imageX[i], 2.0), 0.5);
	}
	
	PlotXY(panelHandle, PANEL_GRAPHFFT, pointsx, results, points_number, VAL_DOUBLE, VAL_DOUBLE, VAL_FAT_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_GREEN);
	
}


/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "task6.1.uir", PANEL));
	
	GetCtrlVal(panelHandle, PANEL_AMPLITUDE, &amplitude);
	GetCtrlVal(panelHandle, PANEL_FREQUENCY, &frequency);
	GetCtrlVal(panelHandle, PANEL_POINTSNUMBER, &points_number);
	GetCtrlVal(panelHandle, PANEL_WAVEFORM, &wave_form);  
    
	
	InitDevices();
	StartGeneration();

    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	
	StopGeneration();
	CloseDevices();

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


int CVICALLBACK TIMERCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			ReadPlotWaveForm();

			break;
	}
	return 0;
}

int CVICALLBACK amplitudeCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panelHandle, PANEL_AMPLITUDE, &amplitude);
			StopGeneration();
			StartGeneration();
			
			break;
	}
	return 0;
}

int CVICALLBACK frequencyCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_FREQUENCY, &frequency);
			StopGeneration();
			StartGeneration();
			break;
	}
	return 0;
}

int CVICALLBACK pointnumberCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_POINTSNUMBER, &points_number);
			StopGeneration();
			StartGeneration();
			break;
	}
	return 0;
}

int CVICALLBACK waveformCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_WAVEFORM, &wave_form);
			StopGeneration();
			StartGeneration();

			break;
	}
	return 0;
}

int CVICALLBACK triggerCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			StartGeneration();
			ReadPlotWaveForm();

			break;
	}
	return 0;
}
