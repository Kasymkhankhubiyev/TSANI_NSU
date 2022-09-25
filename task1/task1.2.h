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
#define  PANEL_GRAPH                      2
#define  PANEL_INPUT_A                    3       /* callback function: A_CHANGE */
#define  PANEL_INPUT_F                    4       /* callback function: F_CHANGE */
#define  PANEL_NUMERICKNOB                5       /* callback function: RAND */
#define  PANEL_TOGGLEBUTTON               6       /* callback function: BUTTON */
#define  PANEL_GRAPH_2                    7
#define  PANEL_TIMER                      8       /* callback function: TIME_CL */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK A_CHANGE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK F_CHANGE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RAND(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIME_CL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
