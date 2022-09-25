#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include "task1.2.h"
#include "toolbox.h"
#include <math.h>

#define N 1000
#define pi 3.14
#define MIN -15
#define MAX 15

static int panelHandle;
static float sinarray[N];
static float fftarr[N];
double f, A, noise; 

void fillarray(){
	for( int i = 0; i < N; i ++){
		sinarray[i] = (float)i/1000.;	
	}
}

void init_params(){
	
	GetCtrlVal(panelHandle, PANEL_INPUT_F, &f);
	GetCtrlVal(panelHandle, PANEL_INPUT_A, &A);
	GetCtrlVal(panelHandle, PANEL_NUMERICKNOB, &noise);
	
}

void make_fourier(){
	double realarr[N], imgarr[N];
	for (int i = 0; i < N; i++){
		imgarr[i]=0;
		realarr[i] = sinarray[i];
	}
	FFT(realarr,imgarr,N);
	
	for (int i = 0; i < N; i++){
		fftarr[i]=sqrt((realarr[i]*realarr[i]) + (imgarr[i]*imgarr[i]));
	}
}

void CreateSignal(){
	fillarray();
		
	for (int i = 0; i < N; i++){
		float res;
		float rand = Random(-1 * abs(noise), abs(noise));
		res = rand + A * sin(2 * Pi() * f * sinarray[i]);
		sinarray[i] = res;
	}
};

void SavePoints(){
	
	FILE* pf;
	
	pf = fopen("GraphPoints.txt", "w");
	
	if(pf == NULL){
	printf("Cannot open file");
	}else{
		for (int i = 0; i < N; i++){
			fprintf(pf, "%d	%f\n", i, sinarray[i]);
		}
		fclose(pf);
	}

}


void DrawGraph(){
	DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
	SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, N);
	SetAxisScalingMode(panelHandle,PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, MIN, MAX);
	PlotY(panelHandle, PANEL_GRAPH, sinarray, N, VAL_FLOAT,VAL_THIN_LINE, VAL_SOLID_SQUARE,VAL_SOLID, 1000, VAL_GREEN); 
};

void DrawGraphFFT(){
	DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
	SetAxisScalingMode(panelHandle,PANEL_GRAPH_2, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, N);
	SetAxisScalingMode(panelHandle,PANEL_GRAPH_2, VAL_LEFT_YAXIS, VAL_MANUAL, MIN, MAX*MAX);
	PlotY(panelHandle, PANEL_GRAPH_2, fftarr, N, VAL_FLOAT,VAL_THIN_LINE, VAL_SOLID_SQUARE,VAL_SOLID, 1000, VAL_GREEN); 
};


int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "task1.2.uir", PANEL));
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
	
	SetCtrlVal(panelHandle,PANEL_INPUT_A,1.);
	SetCtrlVal(panelHandle,PANEL_INPUT_F, 1.);
	init_params();
	CreateSignal();
	make_fourier();
	DrawGraph();
	DrawGraphFFT();
	
    errChk (RunUserInterface ());
	
	//init_params(&A, &f);

	

Error:
    /* clean up */
    DiscardPanel (panelHandle);
    return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK A_CHANGE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			init_params();
			CreateSignal();
			make_fourier();
			DrawGraph();
			DrawGraphFFT();;
			//break;
	}
	return 0;
}

int CVICALLBACK F_CHANGE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			init_params();
			CreateSignal();
			make_fourier();
			DrawGraph();
			DrawGraphFFT();
	}
	return 0;
}

int CVICALLBACK TIME_CL (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			init_params();
			CreateSignal();
			DrawGraph();
			make_fourier();
			DrawGraph();
			DrawGraphFFT();
	}
	return 0;
}

int CVICALLBACK RAND (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK BUTTON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SavePoints();

//break;
	}
	return 0;
}
