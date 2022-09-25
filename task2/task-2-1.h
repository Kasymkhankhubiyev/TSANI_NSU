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
#define  PANEL_NUMERICSLIDE               2       /* callback function: SLIDER1 */
#define  PANEL_NUMERICSLIDE_AI0           3       /* callback function: SLIDER2 */
#define  PANEL_NUMERICSLIDE_AI1           4       /* callback function: SLIDER3 */
#define  PANEL_TEXTMSG                    5
#define  PANEL_NUMERIC222                 6
#define  PANEL_NUMERIC                    7
#define  PANEL_TIMER                      8       /* callback function: TIMER */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SLIDER1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SLIDER2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SLIDER3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
