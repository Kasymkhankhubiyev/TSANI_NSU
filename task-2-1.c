#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "task-2-1.h"
#include "toolbox.h"


static int panelHandle;
static double voltage;
double avg = 0;

int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "task-2-1.uir", PANEL));
	
	ni6251Slot(2);
	analogOut(1, 0.);
	analogOut(0, 0.);
    
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
		analogOut(1, 0.);
		analogOut(0, 0.); 
		ni6251Close();
        QuitUserInterface (0);
	}
    return 0;
}

int CVICALLBACK SLIDER1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panelHandle, PANEL_NUMERICSLIDE, &voltage);
			SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_AI0,voltage);
			SetCtrlVal(panelHandle,PANEL_NUMERICSLIDE_AI1,voltage);
			
			analogOut(1, voltage);
			analogOut(0, voltage);
			
			
	}
	return 0;
}

int CVICALLBACK SLIDER2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK SLIDER3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK TIMER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			double volt;
			analogIn(0, &volt);
			avg = (abs(volt) + avg)/2.0;
			SetCtrlVal(panelHandle,PANEL_NUMERIC222,volt);
			SetCtrlVal(panelHandle,PANEL_NUMERIC,avg);
			
	}
	return 0;
}
