#include "SoftwareSerial.h"
//------------------------------------------------
SoftwareSerial mySerial(10,11);
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define Acknowledge 0x00
#define End_Byte 0xEF

#define ACTIVATED LOW



boolean isPlaying = false;

//------------------------------------------------
void setup(){
  Serial.begin(115200);
  mySerial.begin (9600);
  delay(1000);

}
//------------------------------------------------



void loop () 
{ 
 playFirst();
 Serial.println("First");
 delay(3000);
 Pause();
 Serial.println("pause");
 delay(3000);
 play();
 Serial.println("PLAY");
 delay(3000);
 playNext();
 Serial.println("NEXT");
 delay(4000);
 setVolume(20);
 Serial.println("setVolume:20");
 delay(2000);
 setVolume(10);
 Serial.println("setVolume:10");
 delay(2000);
 
 playADVERT(1);
 Serial.println("playADVERT:1");
 delay(10000);

 
 
 Serial.println("Loop over!");
     
}



//----------播放第一首并全部循环-------//  
void playFirst()
{
  execute_CMD(0x03, 0, 1);
  delay(100);
  setVolume(30);
  execute_CMD(0x11,0,1);  //全部循环
  delay(100);
}
//------------------------//

//----------暂停----------//
void Pause()         
{
  execute_CMD(0x0E,0,0);
  delay(200);
}
//------------------------//
//----------播放----------//
void play()
{
  execute_CMD(0x0D,0,0); 
  delay(200);
}
//------------------------//
//----------下一首--------//
void playNext()
{
  execute_CMD(0x01,0,0);
  delay(500);
}
//------------------------//
//---------上一首----------//
void playPrevious()
{
  execute_CMD(0x02,0,0);
  delay(500);
}
//------------------------//
//--------设置音量----------//
void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(500);
}
//------------------------//  
//--------插播广告文件夹里的顺序为x广告----------//
void playADVERT(int ADnum)
{
  execute_CMD(0x13, 0, ADnum); // Set the volume (0x00~0x30)
  delay(500);
}
//------------------------//

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = {Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte}; //7E FF 06 0D 00 00 01 XX XX EF
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
