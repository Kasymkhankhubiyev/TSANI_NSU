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
#define  PANEL_TOGGLEBUTTON_2             2       /* callback function: TempOnOffTimerCB */
#define  PANEL_TOGGLEBUTTON               3       /* callback function: OnOffTimerCB */
#define  PANEL_STRING_RECIVE              4
#define  PANEL_STRING_SEND                5
#define  PANEL_COMMANDBUTTON_2            6       /* callback function: WordReciveCB */
#define  PANEL_COMMANDBUTTON              7       /* callback function: WordSendCB */
#define  PANEL_TERMOMETR                  8
#define  PANEL_TIMER                      9       /* callback function: LED_timer_CB */
#define  PANEL_STRIPCHART                 10
#define  PANEL_TIMER_2                    11      /* callback function: TempTimerCB */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK LED_timer_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnOffTimerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TempOnOffTimerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TempTimerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WordReciveCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK WordSendCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
