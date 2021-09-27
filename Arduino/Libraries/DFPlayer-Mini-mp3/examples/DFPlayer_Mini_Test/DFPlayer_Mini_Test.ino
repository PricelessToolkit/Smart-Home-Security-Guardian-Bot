/*******************************************************************************
 * DFPlayer_Mini_Mp3, This library provides a quite complete function for      * 
 * DFPlayer mini mp3 module.                                                   *
 * www.github.com/dfrobot/DFPlayer_Mini_Mp3 (github as default source provider)*
 *  DFRobot-A great source for opensource hardware and robot.                  *
 *                                                                             *
 * This file is part of the DFplayer_Mini_Mp3 library.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is free software: you can redistribute it and/or          *
 * modify it under the terms of the GNU Lesser General Public License as       *
 * published by the Free Software Foundation, either version 3 of              *
 * the License, or any later version.                                          *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * DFPlayer_Mini_Mp3 is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public            *
 * License along with DFPlayer_Mini_Mp3. If not, see                           *
 * <http://www.gnu.org/licenses/>.                                             *
 *									       *
 ******************************************************************************/

/*
 *	Copyright:	DFRobot
 *	name:		DFPlayer_Mini_Mp3 test code
 *	Author:		lisper <lisper.li@dfrobot.com>
 *	Date:		2014-05-22
 *	Description:	mp3 library for DFPlayer mini board
 *
 *	this code is test on leonardo
 *	you can try input:
 *	play			//play current music
 *	play 3			//play mp3/0003.mp3 
 *	next			//play next
 *	prev			//paly previous
 *	pause			//pause	current play
 *	stop			//stop current play
 *	state 			//get current state of module
 *	current			//get current track of tf card
 *	volume			//get current volume 
 *	volume 20		//set volume to 20 (0~30)
 *	single open/close 	//set single loop open or close
 *	reply open/close	//set if need reply
 */
 
#include <SoftwareSerial.h>
#include <DFRobot_utility.h>

#include "DFPlayer_Mini_Mp3.h"

#define BUFSIZE 20	//buf size
#define CMDNUM 8	//cmdbuf size

uint8_t buf[BUFSIZE]; //data buffer for read from Serial
char *cmdbuf[CMDNUM]; //for split string from buf

//
void setup () {
        Serial1.begin (9600);
        Serial.begin (9600);
        while (!Serial);

        mp3_set_serial (Serial1);	//set Serial1 for DFPlayer-mini mp3 module 
		delay(1);  //wait 1ms for mp3 module to set volume
        mp3_set_volume (15);
        mp3_get_tf_sum ();
        print_info ();
}

void print_info () {
        Serial.println ("you send:");
        printHex (send_buf, 10);
        delay (100);
        int recv_leng = serialRead (Serial1, recv_buf, 10, 3);
        if (recv_leng) {
                Serial.println ("you get:");
                printHex (recv_buf, recv_leng);
        }
}

//
void loop () {        
        int leng;
        leng = serialRead (Serial1, buf, BUFSIZE, 3); //first read data from Serial1
        if (leng) {
                Serial.print ("=>");
                printHex (buf, leng);
        }
        leng = serialRead (Serial, buf, BUFSIZE, 3); //read command to buf from Serial (PC)
        if (leng) {
                buf[leng] = '\0';
                Serial.println ((char*)buf);
                int cmdleng = split(cmdbuf, (char*)buf, 8); //split command string to cmdbuf
                if (cmdleng >= 1) {
                        if (strcmp (cmdbuf[0], "next") == 0) {
                                mp3_next ();
                                print_info ();
                        } 
			else if (strcmp (cmdbuf[0], "physical") == 0) {
				if (cmdleng == 2) {
					mp3_play_physical (atoi (cmdbuf[1])); //get arguments
				} else {
					mp3_play_physical ();
				}
				print_info ();
			}
			else if (strcmp (cmdbuf[0], "prev") == 0) {
				mp3_prev ();
				print_info ();
                        } 
                        else if (strcmp (cmdbuf[0], "stop") == 0) {
                                mp3_stop ();
                                print_info ();
                        } 
                        else if (strcmp (cmdbuf[0], "pause") == 0) {
                                mp3_pause ();
                                print_info ();
                        } 
                        else if (strcmp (cmdbuf[0], "state") == 0) {
                                mp3_get_state ();
                                print_info ();
                        } 
                        else if (strcmp (cmdbuf[0], "sum") == 0) {
                                mp3_get_tf_sum ();
                                print_info ();
                        } 
                        else if (strcmp (cmdbuf[0], "current") == 0) {
                                mp3_get_tf_current ();
                                print_info ();
                        }
                        else if (strcmp (cmdbuf[0], "volume") == 0) {
				if (cmdleng == 2) {
					mp3_set_volume (atoi (cmdbuf[1]));
				} else {
					mp3_get_volume ();
				}
                                print_info ();
                        }
                        else if (strcmp (cmdbuf[0], "reply") == 0 && cmdleng == 2) {
                                if (strcmp (cmdbuf[1], "open") == 0)
                                        mp3_set_reply (true);
                                else if (strcmp (cmdbuf[1], "close") == 0)
                                        mp3_set_reply (false);
                                print_info ();
                        }
                        else if (strcmp (cmdbuf[0], "play") == 0 && cmdleng == 2) {
				if (cmdleng == 2) {
					mp3_play (atoi (cmdbuf[1]));
				} else {
					mp3_play ();
				}
                                print_info ();
                        }
                        else if (strcmp (cmdbuf[0], "eq") == 0 && cmdleng == 2) {
                                mp3_set_EQ (atoi (cmdbuf[1]));
                                print_info ();
                        }
                        else if (strcmp (cmdbuf[0], "single") == 0 && cmdleng == 2) {
                                if (strcmp (cmdbuf[1], "open") == 0)
                                        mp3_single_loop (true);
                                else if (strcmp (cmdbuf[1], "close") == 0)
                                        mp3_single_loop (false);
                                print_info ();
                        } else {
				Serial.println ("error! no this command");
			}
                } 
        }
}


