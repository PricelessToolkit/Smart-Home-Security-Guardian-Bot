#include <ArduinoJson.h>
#include <DFPlayer_Mini_Mp3.h>

#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX
#endif

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel leds = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800);

String inData;
String Message;
int Commands;
String Volume;
String Track;
int vol;
int tr;

int period10ms = 10;
int period100ms = 100;
int period500ms = 500;
int period1000ms = 1000;
int period2000ms = 2000;
unsigned long time_now = 0;

int MotionPin = 2;
int MotionState = 0;

StaticJsonDocument<64> doc;

void setup(){


  //analogReference(INTERNAL);  
  pinMode(MotionPin, INPUT);
  
  Serial.begin(115200);
  mySerial.begin (9600);
  
  mp3_set_serial (mySerial);	//set softwareSerial for DFPlayer-mini mp3 module 
  delay(10);  //wait 2ms for mp3 module to set volume
  mp3_set_volume (20);
  delay(20);

/*
   mp3_play ();    //start play
   mp3_play (5);  //play "mp3/0005.mp3"
   mp3_next ();   //play next 
   mp3_prev ();   //play previous
   mp3_set_volume (uint16_t volume);  //0~30
   mp3_set_EQ (); //0~5
   mp3_pause ();
   mp3_stop ();
   void mp3_get_state ();   //send get state command
   void mp3_get_volume (); 
   void mp3_get_u_sum (); 
   void mp3_get_tf_sum (); 
   void mp3_get_flash_sum (); 
   void mp3_get_tf_current (); 
   void mp3_get_u_current (); 
   void mp3_get_flash_current (); 
   void mp3_single_loop (boolean state);  //set single loop 
   void mp3_DAC (boolean state); 
   void mp3_random_play (); 
 */
    
  leds.begin();
  leds.setBrightness(50); // 1-255
  leds.show(); // Initialize all pixels to 'off'
  
}



void loop(){
   int Batt = map(analogRead(A2), 650, 867, 0, 100);
   int Light = map(analogRead(A0), 0, 1024, 0, 100);
   MotionState = digitalRead(MotionPin);
   
   doc["sensors0"] = "HaBot";
   doc["Light"] = Light;
   doc["Bat"] = Batt;
   doc["Motion"] = MotionState;

  if(millis() >= time_now + period2000ms){
    time_now += period2000ms;
    serializeJson(doc, Serial);
  }

    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {

          Message = inData.substring(0, inData.length() - 1); // Delet last character "\n"
          Commands = Message.toInt(); // Convert to int
  



          switch (Commands) {
            case 1001: // Searching WiFi
                 for(uint16_t i=0; i<leds.numPixels(); ) {
                    leds.setPixelColor(i, 255, 128, 0);
                    leds.show();

                  if(millis() >= time_now + period100ms){
                    time_now += period100ms;
                    leds.clear();
                    leds.show();
                    i++;
                     
                    }
                }
              break;
            case 1002: // WiFi Connected

                 for(uint16_t i=0; i<leds.numPixels(); ) {
                    leds.setPixelColor(i, 147, 228, 28);
                    leds.show();

                  if(millis() >= time_now + period10ms){
                    time_now += period10ms;
                    leds.clear();
                    leds.show();
                    i++;
                     
                    }
                }
              break;
            case 1003: // MQTT Connection Failed

                 for(uint16_t i=0; i<leds.numPixels(); ) {
                    leds.setPixelColor(i, 255, 0, 0);
                    leds.show();

                  if(millis() >= time_now + period100ms){
                    time_now += period100ms;
                    leds.clear();
                    leds.show();
                    i++;
                     
                    }
                }
              break;
            case 1004: // MQTT Connected
                    
                 for(uint16_t i=0; i<leds.numPixels(); ) {
                    leds.setPixelColor(i, 0, 31, 255);
                    leds.show();

                  if(millis() >= time_now + period10ms){
                    time_now += period10ms;
                    leds.clear();
                    leds.show();
                    i++;
                     
                    }
                }
              break;
            default:
              break;
          }
          
           // mp3251   volume is 25 Track numper 1
          if(Message.substring(0,3)== "mp3"){
            Volume = Message.substring(3,5);
            Track = Message.substring(5,8);
            vol = Volume.toInt();
            tr = Track.toInt();
            mp3_set_volume (vol);
            delay(50);
            mp3_play (tr);
            
          }
          
            inData = ""; // Clear recieved buffer
        }

       time_now = millis();
       

    }


}
