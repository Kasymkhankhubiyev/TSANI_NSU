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
    errChk (panelHandle = LoadPanel (0, "task-2-2.uir", PANEL));
	
	ni6251Slot(2); 
    
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

			double v2, v3;
			analogIn(2, &v2);
			analogIn(3, &v3);
			SetCtrlVal(panelHandle,PANEL_NUMERIC,v2);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_2,v3);
			v2 = (1.0/2.5)*v2 - 1.0;
			v3 = (1.0/2.5)*v3 - 1.0;
			SetCtrlVal(panelHandle,PANEL_NUMERIC_3,v2);
			SetCtrlVal(panelHandle,PANEL_NUMERIC_4,v3);
			//DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, -1, 1);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -1, 1);
			PlotPoint(panelHandle, PANEL_GRAPH, v2, v3, VAL_SOLID_SQUARE, VAL_GREEN);
			
	}
	return 0;
}
