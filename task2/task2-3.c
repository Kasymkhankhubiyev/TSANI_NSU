//==============================================================================
//
// Title:       task2-3
// Purpose:     A short description of the application.
//
// Created on:  28.09.2022 at 16:07:52 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "task2-3.h"
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
    errChk (panelHandle = LoadPanel (0, "task2-3.uir", PANEL));
	
	portMask(2, 7);
	portMask(2, 1);
	
#define mask0 4095 //1111111111110000
#define mask1 15 //0000000000001111
	
	portMask(0, mask0);
	portMask(1, mask1);
    
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

int runlight = 0;

int s1prev = 1;
int s2prev = 1;
int s3prev = 1;
int s4prev = 1;

int port_num = 0;
int diod_bit = 4;
int diod_num_prev = 0;

int CVICALLBACK timeCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			unsigned char data;
			portIn(1, &data);
			int s1 = data & (1 << 4); //0 - s1 нажата, 1 - отпущена
			int s2 = data & (1 << 5); //0 - s2 нажата, 1 - отпущена 
			int s3 = data & (1 << 6); //0 - s3 нажата, 1 - отпущена 
			int s4 = data & (1 << 7); //0 - s4 нажата, 1 - отпущена
			
			/*
			int diodnum = 0;
			
			GetCtrlVal(panel,PANEL_LISTBOX,&diodnum);
			
			if ((!diodnum) && diod_num_prev){
				if (diodnum > 7){
					port_num = 1;
					diod_bit = diodnum - 8;
				}else{
					port_num = 0;
					diod_bit = diodnum;
				}
			}
			*/
			
			if((!s1) && s1prev){
				portIn(port_num, &data);
				data ^= 1<<diod_bit;
				portOut(port_num, data);
			}
			
			if((!s2) && s2prev){
				runlight = 1;
				SetCtrlAttribute(panel, PANEL_TIMER_2, ATTR_ENABLED, 1);
			
			}
			
			if ((!s3) && s3prev){
				SetCtrlAttribute(panel, PANEL_TIMER_2, ATTR_ENABLED, 0);
				portOut(0, 0);
				portOut(1, 0);
			}
			
			if((!s4) && s4prev){
				runlight = 0;
				SetCtrlAttribute(panel, PANEL_TIMER_2, ATTR_ENABLED, 1);
			
			}
			
			s1prev = s1;
			s2prev = s2;
			s3prev = s3;
			s4prev = s4;
			

			break;
	}
	return 0;
}

int CVICALLBACK listevent (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int cur_port = 0;
int cur_bit = 0;

int CVICALLBACK runlightCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			unsigned char data;
			
			if (runlight){
				portOut(0, 0);
				portOut(1, 0);
				
				if (cur_port){
					if (cur_bit == 3){
						portOut(cur_port, 1<<cur_bit);
						cur_port = 0;
						cur_bit = 0;
					}else{
						portOut(cur_port, 1<<cur_bit);
						cur_bit++;
					}
				}else{
					if (cur_bit == 7){
						portOut(cur_port, 1<<cur_bit);
						cur_port = 1;
						cur_bit = 0;
					}else{
						portOut(cur_port, 1<<cur_bit);
						cur_bit++;
					}
				}
			}else{
				portOut(0, 0);
				portOut(1, 0);
				if (cur_port){
					if (cur_bit == 0){
						portOut(cur_port, 1<<cur_bit);
						cur_port = 0;
						cur_bit = 7;
					}else{
						portOut(cur_port, 1<<cur_bit);
						cur_bit--;
					}
				}else{
					if (cur_bit == 0){
						portOut(cur_port, 1<<cur_bit);
						cur_port = 1;
						cur_bit = 3;
					}else{
						portOut(cur_port, 1<<cur_bit);
						cur_bit--;
					}
				}
			}

			break;
	}
	return 0;
}
