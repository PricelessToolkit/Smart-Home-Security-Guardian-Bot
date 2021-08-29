
#include <ArduinoJson.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "Arduino.h"
#define LED_BUILTIN 4


const int fwdPin = 2;  
const int turnPin = 12;  

// Arduino like analogWrite
// value has to be between 0 and valueMax
void fwdAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 2000) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}
void steeringAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 2000) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

const char* ssid = "Your_wifiname";
const char* password = "Your_password";
const char* mqtt_username = "Your_mqtt_username";
const char* mqtt_password = "Your_mqtt_password";
const char* mqtt_server = "Your_mqtt/homeassistant server IP";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
	  
    steeringAnalogWrite(2, 0); // STOP when WIFI is not connected
    steeringAnalogWrite(4, 0); // STOP when WIFI is not connected
	Serial.println(1001); // Searching WIFI
    delay(500);
    
  }

  Serial.println(1002); // WiFi Is Connected


    // Start streaming web server
  startCameraServer();
}




#define PART_BOUNDARY "123456789000000000000987654321"

// This project was tested with the AI Thinker Model, M5STACK PSRAM Model and M5STACK WITHOUT PSRAM

  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22


static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}


void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
  
  //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }
}

void reconnect() {
  while (!client.connected()) {

    Serial.println(1003); // Conectioon Faild
    steeringAnalogWrite(2, 0); // STOP when MQTT is not connected
    steeringAnalogWrite(4, 0); // STOP when MQTT is not connected
 
    // Create a random client ID
    String clientId = "HABot";

    // Attempt to connect
    if (client.connect("HABot", mqtt_username, mqtt_password)) {

      client.subscribe("HABot/Commands/Steering");
      client.subscribe("HABot/Commands/Data");
      Serial.println(1004); // MQTT Connected


      
    }
    else
    {
      // Wait 0.1 seconds before retrying
      delay(100);
    }
  }
}

void MQTTCallback(char* topic, byte* payload, unsigned int length) {
  String Command = "";

  for (int i = 0; i < length; i++) {
    Command = Command + (char)payload[i];
  }  

  Serial.println(Command); // Send all Data to arduino
  
  int data = Command.toInt();
  
  if(strcmp(topic, "HABot/Commands/Steering") == 0)
  {
	if (Command == "right")
	{
	steeringAnalogWrite(4, 1120);
	steeringAnalogWrite(2, 1220);
	}
   


	if (Command == "left")
	{
	steeringAnalogWrite(2, 1150);
	steeringAnalogWrite(4, 1200);
	}


   if (Command == "forward")
    {
    steeringAnalogWrite(2, 1022);
    steeringAnalogWrite(4, 1000);
    }

   if (Command == "backward")
    {
    steeringAnalogWrite(2, 1240);
    steeringAnalogWrite(4, 1218);
    }

    if (Command == "stop")
    {
    steeringAnalogWrite(2, 0);
    steeringAnalogWrite(4, 0);
    }
    
  }

}


void DeserializAndPub(){
  
     StaticJsonDocument<128> doc;
     while (Serial.available() > 0)
    {
    DeserializationError error = deserializeJson(doc, Serial);

    if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
    }

    const char* sensors = doc["sensors"]; // "HaBot"
    const char* Light = doc["Light"]; // "210"
    const char* Bat = doc["Bat"]; // "10"
    const char* Motion = doc["Motion"]; // "HIGH"
    char buffer[128];
    size_t n = serializeJson(doc, buffer);
    client.publish("HABot/sensors", buffer, n);
    client.endPublish();


        }
    

    return;
}

void setup(){



  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT);
  pinMode (LED_BUILTIN, LOW);


  // forward motor PWM
  ledcSetup(2, 50, 16); //channel, freq, resolution
  ledcAttachPin(fwdPin, 2); // pin, channel
 
  // steering servo PWM
  ledcSetup(4, 50, 16); //channel, freq, resolution
  ledcAttachPin(turnPin, 4); // pin, channel 

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
  client.setCallback(MQTTCallback);  

    camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 25;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 25;
    config.fb_count = 1;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
}

void loop(){
      if (!client.connected()) 
        {
            reconnect();
        }
        
    DeserializAndPub();

  client.loop();
}
