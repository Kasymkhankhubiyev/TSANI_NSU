//==============================================================================
//
// Title:       task4
// Purpose:     A short description of the application.
//
// Created on:  16.11.2022 at 19:04:14 by TC.
// Copyright:   ANY. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "task4.h"
#include "toolbox.h"

//==============================================================================
// Constants
#define DELAY 0.001
//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;
char name[7] = {0};

int flag = 0;
int data = 0x01;

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

int check_value_int(int* value, int min, int max)
{
    /*Checks if a value fit in the range,
    if not it cuts of fills the value*/
	if (*value > max)
	{
		*value = max;
	}
	else if (*value < min)
	{
		*value = min;
	}
	
	return 0;
}

void i2c_init(void)
{
	portMask(0, 0x1F); // left bit is zero because of port0/line7 (read SDAin), rest on port0 for write
	portMask(1, 0x00); // all the port1 lines are used for reading
	portOut(0, 0x07);
	portOut(1, 0x00);
	
	/*portMask(0, 0x1F);
	portMask(1, 0x00);
	portOut(0,0x07);*/
} 

void i2c_write(int sda, int scl)
{
	unsigned char data;
	data = (sda << 3) | (scl << 4) | 0x07; // SDAout and SCLout for I2C-1
	data = data | (1 << 5) | (1 << 6); //holding SDAout and SCLout at high voltage for I2C-2 for advance
	portOut(0, data);
}

void i2c_read(int* sda, int* scl)
{
	unsigned char temp1, temp2;
	portIn(0, &temp1);
	portIn(1, &temp2);
	*sda = (temp1 >> 7) & 1; // port0/line7 - SDAin for I2C-1
	*scl = temp2 & 1;		 // port1/line0 - SCLin for I2C-1
}

void i2c_start()
{
	i2c_write(1, 1);
	Delay(DELAY);
	i2c_write(0, 1);
	Delay(DELAY);
	i2c_write(0, 0);
	Delay(DELAY);
}

void i2c_stop()
{
	i2c_write(0, 1);
	Delay(DELAY);
	i2c_write(1, 1);
	Delay(DELAY);
	i2c_write(0, 0);
	Delay(DELAY);
}

int i2c_sda()
{
	unsigned char temp;
	portIn(0, &temp);
	return (temp >> 7) & 0x01; // Current SDA value
}

void i2c_send_bit(int bit)
{
    /*In this tack we perform a bit transaction through SDA voltage*/
	i2c_write(i2c_sda(), 0); //pick up current state of SDA (data does not changes freeing SCL) and free SCL
	i2c_write(bit, 0); //change SDA value with bit's value 
	Delay(DELAY);
	
	//i2c_write(bit, 1); //Raise SCL keeping SDA, signalizing start tact. 
	i2c_write(i2c_sda(), 1);
	Delay(DELAY);
	
	//i2c_write(bit, 0); // Decrise SCL keeping SDA, signalizing stop tact. 
	i2c_write(i2c_sda(), 0);
	Delay(DELAY);	  
}

void i2c_send_byte(int byte)
{
	int i;
	for(i = 0; i < 8; i++) i2c_send_bit((byte >> (7-i)) & 0x01); // send a row of 8 bits from our byte,
    //transaction runs from high bit to small bit
}

int i2c_receive_ACK() 
{	
    /*Accepting each byte (8 bits) acceptor has to send back an acknowledge
    Current function runs right after the byte was sent,
    because in this function we try to get an aclnowledge bit.
    Leading controls tact generation and acception, and sending*/
	double time = 0;
	i2c_write(i2c_sda(), 0);
	Delay(DELAY);
	
	i2c_write(0, 0); //Free SDA	//////////////////
	Delay(DELAY);
	
	i2c_write(0, 1); //Free SDA ////////////////// (1, 1) ?
	
	while (i2c_sda() == 1)
	{
		Delay(DELAY);
		time += DELAY;
		if(time > 1) return 1; // If waiting time is more than 1 sec, send an error
	}
	Delay(DELAY);
	i2c_write(0, 0);

	return 0;
}

void i2c_send_ACK()
{
    /*This function runs when we got 1 byte and
    we want to send acknowledge bit */
	i2c_send_bit(0);
}

void i2c_send_NACK()
{   /*Signalizing an Error*/
	i2c_send_bit(1);
}

int i2c_receive_bit()
{
	int bit;
	i2c_write(1, 0); ////////////////// Key disconection?
	Delay(DELAY);
	i2c_write(1, 1);
	Delay(DELAY);
	bit = i2c_sda();
	Delay(DELAY);
	i2c_write(1, 0);
	return bit;
}

void i2c_receive_byte(int* byte)
{
	int i, temp = 0;
	for(i = 0; i < 8; i++)
	{
		temp = temp | (i2c_receive_bit() << (7 - i));
	}
	*byte = temp;
}

int i2c_write_word(int address, int subaddress, int word)
{
	check_value_int(&address, 0, 127);
	address = address << 1; // left 7 bits - address, last bit equals 0 - it means to expect writing via this address 
	check_value_int(&subaddress, 0, 255);
	check_value_int(&word, 0, 255);
	
	i2c_start();
	
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(subaddress);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(word);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_stop();

	return 0;
}

int i2c_read_word(int address, int subaddress, int* data, int n) //n - bytes amount for one transaction
{
	int temp, i;
	check_value_int(&address, 0, 127);
	if(n < 1) n = 1; // if someone sets negative value on bytes amount
	address = address << 1; // left 7 bits - address, last bit eauals 0 - it means to expect writing via this address
	check_value_int(&subaddress, 0, 255);
	
	i2c_start();
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_send_byte(subaddress);  //Send a subaddress to read further via this subaddress
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_stop();
	
	i2c_start();
	address = address | 0x01; // Last bit eauals 1 - in means to expect reading via this address
	
	i2c_send_byte(address); //
	if(i2c_receive_ACK() != 0) return 1;
	
	for(i = 0; i < n; i++)
	{
		i2c_receive_byte(data + i); //read data via pointer (might be as an array for n>1, and a variable for n=1)
		if(i != (n-1)) i2c_send_ACK(); //"Leading sets ACK after each byte, and sets NACK after the last byte."
	}
	i2c_send_NACK(); 
	
	i2c_stop();

	return 0;
}

int i2c_read_temperature(double* value)
{				
	
	//unsigned int o_data, y_data;
	//int code;
	
	int temp, data = 0;
	int address = 0x28; //Termosensor address- 0x28. Gives 2 bytes of data, use bytes from 15 to 6: 0000 0000 00XX XXXX  
	address = (address << 1) | 0x01; //We read via this address
	
	i2c_start();
	
	i2c_send_byte(address);
	if(i2c_receive_ACK() != 0) return 1;
	
	i2c_receive_byte(&temp);
	i2c_send_NACK();

	data = temp << 2; //Termosensor returns 10 bits of data via 2 bytes (16 bits). 8 of them we have already got - lets write them
	
	i2c_receive_byte(&temp);
	i2c_send_NACK();
	
	data = data | (temp >> 6); // write rest 2 bits, in sum 10 bits

	i2c_stop();
	
	if(data >= 512)
		data = data - 1024; //data intepretation
	
	*value = (double)data * 0.25; //Temperature in degrees
	
	
	return 0;
}

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "task4.uir", PANEL));
	
	//ni6251Slot(2);
	i2c_init();
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	
	//ni6251Close();

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

int CVICALLBACK OnOffTimerCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int state;
			GetCtrlVal(panel,PANEL_TOGGLEBUTTON, &state);
			SetCtrlAttribute(panel, PANEL_TIMER, ATTR_ENABLED, state);

			break;
	}
	return 0;
}



int CVICALLBACK LED_timer_CB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			//date = ((date & 0x80) >> 7) | ((date << 1));
			data = ((data >> 7) | (data << 1)) & 0xff;
			i2c_write_word(0, 0x00, data);

	}
	return 0;
}


void readName(){
	int letter = 0;
	for (int i = 0; i < 7; i++){
		i2c_read_word(0, i+8, &letter, 1);
		name[i] = (char) letter;
	}

}

int CVICALLBACK WordSendCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK WordReciveCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			readName();
			SetCtrlVal(panelHandle, PANEL_STRING_RECIVE, name);

			break;
	}
	return 0;
}


int CVICALLBACK TempOnOffTimerCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int state;
			GetCtrlVal(panel,PANEL_TOGGLEBUTTON_2, &state);
			SetCtrlAttribute(panel, PANEL_TIMER_2, ATTR_ENABLED, state);
			break;
	}
	return 0;
}

int CVICALLBACK TempTimerCB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			double value;
			i2c_read_temperature(&value);
			SetCtrlVal(panelHandle, PANEL_TERMOMETR, value);
			//SetAxisScalingMode(panel, PANEL_STRIPCHART,VAL_LEFT_YAXIS, VAL_AUTOSCALE, -50.0, 50.0);
			PlotStripChartPoint(panelHandle, PANEL_STRIPCHART, value);

			break;
	}
	return 0;
}

		  

