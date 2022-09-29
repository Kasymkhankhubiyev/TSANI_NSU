//==============================================================================
//
// Title:       task-2-2
// Purpose:     A short description of the application.
//
// Created on:  21.09.2022 at 19:09:39 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "task-2-2.h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

#define N 20

static int panelHandle;
static float pointsX[N];
static float pointsY[N];
int counter = 0;


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
    errChk (panelHandle = LoadPanel (0, "task-2-2.uir", PANEL));
	
	ni6251Slot(2);
	for (int i = 0; i < N; i++){
		pointsX[i] = 10.;
		pointsY[i] = 10.;
	}
    
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
    if (event == EVENT_CLOSE){
		ni6251Close();
        QuitUserInterface (0);
		
	}
    return 0;
}

int CVICALLBACK TIMER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			double vx, vy;
			analogIn(2, &vx);
			analogIn(3, &vy);
			SetCtrlVal(panelHandle,PANEL_NUMERIC,vx);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_2,vy);
			vx = -1.0 * ((1.0/2.4)*vx - 1.0);  //x
			vy = (1.0/2.38)*vy - 1.0;		   //y
			if (counter == N){
				counter = 0;
				pointsX[counter] = vx;
				pointsY[counter] = vy;
				counter++;
			}else{
				pointsX[counter] = vx;
				pointsY[counter] = vy;
				counter++;
			}
			SetCtrlVal(panelHandle,PANEL_NUMERIC_3,vx);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_4,vy);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, -1.1, 1.1);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -1.1, 1.1);
			for (int i = 0; i < N; i++){
				PlotPoint(panelHandle, PANEL_GRAPH, pointsX[i], pointsY[i], VAL_SOLID_SQUARE, VAL_GREEN);
			}
	}
	return 0;
}




