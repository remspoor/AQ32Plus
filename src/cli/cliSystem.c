/*
  August 2013

  Focused Flight32 Rev -

  Copyright (c) 2013 John Ihlein.  All rights reserved.

  Open Source STM32 Based Multicopter Controller Software

  Designed to run on the AQ32 Flight Control Board

  Includes code and/or ideas from:

  1)AeroQuad
  2)BaseFlight
  3)CH Robotics
  4)MultiWii
  5)Paparazzi UAV
  5)S.O.H. Madgwick
  6)UAVX

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////////////////////////

#include "board.h"

///////////////////////////////////////////////////////////////////////////////
// Telemetry CLI
///////////////////////////////////////////////////////////////////////////////

void systemCLI()
{
    USART_InitTypeDef USART_InitStructure;

	char     baudString[7];
	int		 i;

	uint8_t  systemQuery = 'x';
    uint8_t  validQuery = false;

    cliBusy = true;

    cliPortPrint("\nEntering System CLI....\n\n");

    while(true)
    {
        cliPortPrint("System CLI -> ");

            while ((cliPortAvailable() == false) && (validQuery == false));

	    if (validQuery == false)
		systemQuery = cliPortRead();

	    cliPortPrint("\n");

	    switch(systemQuery)

	    {
            ///////////////////////////

            case 'a': // System Configuration
                cliPortPrint("\nSystem Configuration:\n");
                cliPortPrintF("USART1 settings: %6l,%4h,%4h,%4h,%4h,%4h\n",
                		USART_InitStructure.USART_BaudRate,
                		USART_InitStructure.USART_WordLength,
                		USART_InitStructure.USART_StopBits,
                		USART_InitStructure.USART_Parity,
                		USART_InitStructure.USART_Mode,
                		USART_InitStructure.USART_HardwareFlowControl);



                validQuery = false;
                break;

            ///////////////////////////

            case 'B': // Read baudrate
				readStringCLI( baudString, 6 );

				cliPortPrint("Baudrate received by command: ");
				cliPortPrint(baudString);

				i = atoi(baudString);

				cliPortPrintF("\nReinitialize USART1 with %6l baud\n", i);
				USART_InitStructure.USART_BaudRate = i;
				USART_Init(USART1, &USART_InitStructure);
				cliPortPrint("done..\n");


                validQuery = false;
        	    break;

            ///////////////////////////

            case 'R': // Reset to Bootloader
    			cliPortPrint("Entering Bootloader....\n\n");
    			delay(100);
    			systemReset(true);
    			break;

    		///////////////////////////

    		case 'S': // Reset System
    			cliPortPrint("\nSystem Reseting....\n\n");
    			delay(100);
    			systemReset(false);
    			break;

    		///////////////////////////

			case 'x':
			    cliPortPrint("\nExiting System CLI....\n\n");
			    cliBusy = false;
			    return;
			    break;

            ///////////////////////////

            case 'W': // Write EEPROM Parameters
                cliPortPrint("\nWriting EEPROM Parameters....\n\n");
                writeEEPROM();

                validQuery = false;
                break;

    		///////////////////////////

            case '?':
			   	cliPortPrint("\n");
                cliPortPrint("'a' Display system parameters\n");
                cliPortPrint("                                           'B' Set USART1 baudrate    Bxxxxxx\n");
                cliPortPrint("                                           'C' Set USART2 baudrate    Cxxxxxx\n");
                cliPortPrint("                                           'D' Set USART3 baudrate    Dxxxxxx\n");
                cliPortPrint("\n");
				cliPortPrint("                                           'R' Reset and Enter Bootloader\n");
				cliPortPrint("                                           'S' Reset\n");
			   	cliPortPrint("\n");
   		        cliPortPrint("                                           'W' Write EEPROM Parameters\n");
   		        cliPortPrint("'x' Exit Telemetry CLI                     '?' Command Summary\n\n");
   		        break;

	    	///////////////////////////
	    }
	}
}

///////////////////////////////////////////////////////////////////////////////
