// Hantek-CLI.cpp : Defines the entry point for the console application.
//

#define _X86_

#include "stdafx.h"
#include <tchar.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include <conio.h>
#include <iostream>


#define BUF_SIZE	4096
#define MAX_VOLT	3.5f
#define MEA_FREQ	0
#define MEA_COUNT	1

USHORT buffer[BUF_SIZE];
float	m_volt = 3.0f;
float m_offset = 0.0f;
int m_nWavePointNum = 2048;
int m_nWavePeriodNum = 1;
double	m_frequency;
int m_iDevice = 0;


void CreateSineWave(float p_volt)
{
	int i;
	float tmp;
	for (i = 0; i < m_nWavePointNum; i++)
	{
		tmp = float(m_offset*1.0 / MAX_VOLT
			+ p_volt / MAX_VOLT * sin((i*1.0 / m_nWavePointNum) * 2 * 3.14159f*m_nWavePeriodNum));
		tmp = 2048 - 2047 * tmp;
		if (tmp > 4095)
			tmp = 4095;
		else if (tmp < 0)
			tmp = 0;
		buffer[i] = USHORT(tmp);
	}
}


int main()
{
	int result = DDSSearch();
	if (result == 0) {
		_tprintf(_T("Device not connected, Exiting!!!"));
		return 1;
	}
// We need to create the wave shape and download it to the device.
	bool firstSetFreq = DDSSetFrequency(m_iDevice, 1.0000000000000000, &m_nWavePointNum, &m_nWavePeriodNum);
	CreateSineWave(m_volt);
	DDSDownload(m_iDevice, buffer, m_nWavePointNum);

// Now we build our loop and control structure. 
	bool running = true;
	bool paused = true;
	bool incrementTrueElseDecrement = true;
	char key;
	char lastkey = 0;
	m_frequency = 0.0;
	int mytimer = 0;
	int mytimerB = 0;
	int ticksToSec = 10000;
	int waitSecs = 5;
	double freqIncrement = 1.0;
	bool incrementNextLoop = false;
	bool redrawConsole = true;
	bool stuckKey = false;
	
	while (running) {
		// we should readkeys pressed  
		if (redrawConsole) {
			// Draw out our console dashboard
			system("cls");
			printf("   ____ ____ ____ _________ ____ ____ ____ \n");
			printf("  ||D |||D |||S |||       |||C |||L |||I ||\n");
			printf("  ||__|||__|||__|||_______|||__|||__|||__||\n");
			printf("  |/__\\|/__\\|/__\\|/_______\\|/__\\|/__\\|/__\\|\n\n");
			printf("\n"
				" Use Spacebar to Pause and C to continue \n"
				" Use < to sweep down and > to sweep up\n"
				" Use - to lower step value and + to increase step value\n"
				" Use [ to lower dwell time and ] to increase dwell time \n"
				" Use ( to lower volts and ) to increase \n"
				" Press E to enter Frequency \n"
				"\n\n");
			printf("         Paused Status: %s \n", paused ? "true" : "false");
			printf("    Sweep Incrementing: %s \n", incrementTrueElseDecrement ? "true" : "false");
			printf("               voltage: %f \n", m_volt);
			printf("    Increment Value By: %f \n",  freqIncrement);
			printf("          Dwell Timing: %d \n", waitSecs);
			printf(" Current Frequency(hz): %f \n", m_frequency);
			printf("                       |hz |kh|mh|");
			redrawConsole = false;
		}

		key = 0;
		HANDLE peekH = GetStdHandle(STD_INPUT_HANDLE);
		DWORD events;
		INPUT_RECORD peekBuff;
		PeekConsoleInput(peekH, &peekBuff, 1, &events);
		if (events > 0) {
			ReadConsoleInput(peekH, &peekBuff, 1, &events);
			key = peekBuff.Event.KeyEvent.wVirtualScanCode/*wVirtualKeyCode*/;
		}

		if (lastkey != 0) {
			if (key == lastkey) {
				stuckKey = true;
				//printf("Stuck Key Error");
			}
		}

		switch (key) {
		case 10: // (
			if (stuckKey) {
				stuckKey = false;
			}
			else {
				m_volt = m_volt - 0.1;
				if (m_volt < 0) {
					m_volt = 0;
				}
				redrawConsole = true;
			}
			break;
		case 11: // )
			if (stuckKey) {
				stuckKey = false;
			}
			else {
				m_volt = m_volt + 0.1;
				if (m_volt > 3.5) {
					m_volt = 3.5;
				}
				redrawConsole = true;
			}
			break;
		case 12: // - 
			if (stuckKey) {
				stuckKey = false;
			} 
			else {
				if (freqIncrement < 10) {
					freqIncrement--;
				} 
				else {
					if (freqIncrement < 100) {
						freqIncrement = freqIncrement - 10;
					}
					else {
						if (freqIncrement < 1000) {
							freqIncrement = freqIncrement - 100;
						}
						else {
							freqIncrement = freqIncrement - 1000;
						}
					}
				}
				redrawConsole = true;
			}
			break;
		case 13: // +
			if (stuckKey) {
				stuckKey = false;
			}
			else {
				if (freqIncrement < 10) {
					freqIncrement++;
				}
				else {
					if (freqIncrement < 100) {
						freqIncrement = freqIncrement + 10;
					}
					else {
						if (freqIncrement < 1000) {
							freqIncrement = freqIncrement + 100;
						}
						else {
							if (freqIncrement < 10000) {
								freqIncrement = freqIncrement + 1000;
							}
							else {
								if (freqIncrement < 100000) {
									freqIncrement = freqIncrement + 10000;
								}
								else {
									freqIncrement = freqIncrement + 100000;
								}
							}
						}
					}
				}
				redrawConsole = true;
			}
			break;
		case 18: // e
			// read value
			system("cls");
			printf("\n Enter value for Frequency and press enter;\n\n");
			float valInput;
			scanf_s("%f", &valInput);
			m_frequency = valInput;
			redrawConsole = true;
			break;
		case 26: // [
			if (stuckKey) {
				stuckKey = false;
			}
			else {
				waitSecs--;
				redrawConsole = true;
			}
			break;
		case 27: // ]
			if (stuckKey) {
				stuckKey = false;
			}
			else {
				waitSecs++;
				redrawConsole = true;
			}
			break;
		case 46: // c
			paused = false;
			redrawConsole = true;
			break;
		case 57: // spacebar
			paused = true;
			redrawConsole = true;
			break;
		case 52: // >
			incrementTrueElseDecrement = true;
			redrawConsole = true;
			break;
		case 51: // <
			incrementTrueElseDecrement = false;
			redrawConsole = true;
			break;
		case 1: // esc
			return 0;
			break;
		case 0:
			break;
		default:
			printf("key was: %d \n", +key);
		}
		lastkey = key;
		key = 0;

		if (paused) {
			// Skip frequency change
		}
		else {
			if (incrementNextLoop) {
				// modify our frequency
				if (incrementTrueElseDecrement) {
					// increment value
					m_frequency = m_frequency + freqIncrement;
					// test not above X
					if (m_frequency > 25000000.0) {
						m_frequency = 25000000.0;
					}
				}
				else {
					// decrement value
					m_frequency = m_frequency - freqIncrement;
					// test not below 0
					if (m_frequency < 0.0) {
						m_frequency = 0;
					}
				}
				CreateSineWave(m_volt);
				DDSDownload(m_iDevice, buffer, m_nWavePointNum);
				bool setFreq = DDSSetFrequency(m_iDevice, m_frequency, &m_nWavePointNum, &m_nWavePeriodNum);
				incrementNextLoop = false;
				redrawConsole = true;
			}
		}
		
		// about 10000 increments is a second
		mytimer++; 
		if (mytimer > ticksToSec) {
			mytimer = 0;
			mytimerB++;
		}
		if (mytimerB > waitSecs) {
			mytimerB = 0;
			incrementNextLoop = true;
		}	
	}
    return 0;
}

