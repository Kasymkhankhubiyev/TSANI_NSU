/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2022. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_NUMERICTHERM               2       /* callback function: cbtermometr */
#define  PANEL_TOGGLEBUTTON               3       /* callback function: BTNcb */
#define  PANEL_LED                        4
#define  PANEL_NUMERIC                    5
#define  PANEL_CONTROLBUTTON              6       /* callback function: controlcb */
#define  PANEL_TIMER                      7       /* callback function: cbtimer */
#define  PANEL_STRIPCHART                 8
#define  PANEL_STIMER                     9       /* callback function: stimer */
#define  PANEL_CONTROLTIMER               10      /* callback function: timecontrol */
#define  PANEL_NUMERIC_2                  11
#define  PANEL_TIMER_2                    12      /* callback function: timebig */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BTNcb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbtermometr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbtimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK controlcb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK stimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timebig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timecontrol(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
