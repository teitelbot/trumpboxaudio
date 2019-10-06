#include <Servo.h>                        //包含舵机库
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
const char* ssid = "***";
const char* password = "***";

#define PIN_BUSY D8  // not using this pin for now so you dont need to wire anything to this pin
SoftwareSerial mySoftwareSerial(D1, D2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;


Servo handServo;                          //定义舵机名称
Servo boxServo;

int switchStatus=0, action=1, vot, randNumber=0;                   //定义基本整形变量用于存储开关状态 电池电压 开关次数和种类
const int ledPin = D0;                    //定义LED引脚
const int frontSwitchPin = D7;
const int handServoPin = D5;
const int boxServoPin = D6;


void setup()
{
   Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.hostname("uselessbox");
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(ledPin, OUTPUT);                  //定义LED引脚为输出
  digitalWrite(ledPin,HIGH);
  
  pinMode (D7, INPUT_PULLUP);
  handServo.attach(handServoPin);              //定义舵机信号引脚
  boxServo.attach(boxServoPin);
  handServo.write(190);                        //舵机复位
  boxServo.write(70);

  //randomSeed(analogRead(0));
  
   mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP806 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume (15);
}


void loop()
{
   ArduinoOTA.handle();
  switchStatus = digitalRead(frontSwitchPin); //读取开关状态
  //action = random(1,16);
  
  if (switchStatus == LOW){                   //若开关被按下
    
    if (action == 1)
    {
            Serial.println("Action 1");           //第一种 慢慢抬头看 慢慢关开关
      for (int i = 70; i <= 130; i++)
      {
        boxServo.write(i);
        delay(20);
      }
myDFPlayer.play (3);
Serial.print("have i done a good job");
Serial.println(digitalRead(PIN_BUSY));
delay (1000);
      for (int i = 190; i >= 35; i--)
      {
      handServo.write(i);
        delay(20);
      }
      delay(1000);
      handServo.write(0);
      delay(200);
      for (int i = 0; i <=190; i++)
      {
        handServo.write(i);
        delay(20);
      }
      for (int i = 130; i >= 70; i--)
      {
        boxServo.write(i);
        delay(20);
      }
      action++;
    }
    
    
    else if (action == 2)
    {
      Serial.println("Action 2");         //第二种 迅速关开关 事后探头看
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
      delay(550);
myDFPlayer.play (10);
Serial.print("everybody knows what im saying: ");
Serial.println(digitalRead(PIN_BUSY));
delay (1000);
      handServo.write(0);
      delay(1500);
      handServo.write(190);
      delay(550);
      boxServo.write(70);
      delay(1500);
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
      delay(3000);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 3)
    {
      Serial.println("Action 3");         //第三种 抬头看一下 然后迅速关开关
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
      delay(1000);
myDFPlayer.play (4);
Serial.print("make american great again");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      boxServo.write(70);
      delay(29);
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
      handServo.write(0);
      delay(850);
      handServo.write(190);
      delay(500);
      boxServo.write(70);
      action++;
    }
   
    else if (action == 4)
    {
      Serial.println("Action 4");         //第四种 慢慢抬头看 手碰开关 然后迅速关开关
      for (int i = 70; i <= 130; i++)
      {
        boxServo.write(i);
        delay(20);
      }
myDFPlayer.play (2);
Serial.print("that was such a great speech: ");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      for (int i = 190; i >= 35; i--)
      {
        handServo.write(i);
        delay(20);
      }
      delay(29);
      handServo.write(0);
      delay(500);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 5)
    {
      Serial.println("Action 5");       //第五种 抬头看然后挥手抗议
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (5);
Serial.print("a great wall: ");
Serial.println(digitalRead(PIN_BUSY));
delay (6000);
      delay(29);
      handServo.write(65);
      delay(50);
      handServo.write(40);
      delay(20);
      handServo.write(65);
      delay(20);
      handServo.write(40);
      delay(20);
      handServo.write(65);
      delay(20);
      handServo.write(40);
      delay(20);
      handServo.write(65);
      delay(20);
      handServo.write(40);
      delay(20);
      handServo.write(65);
      delay(20);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(29);
      handServo.write(0);
      delay(600);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 6)             
    {
      Serial.println("Action 6");       //第六种 抬头看然后摔盒子 然后关开关 然后探头看
      for (int i = 70; i <130; i++)
      {
        boxServo.write(i);
        delay(20);
      }
myDFPlayer.play (6);
Serial.print("your fired: ");
Serial.println(digitalRead(PIN_BUSY));
delay (20);
      delay(1000);
      for (int i = 0; i < 6; i++)
      {
        for (int j = 70; j <= 130; j++){
          boxServo.write(j);
          delay(6);
        }
        delay(200);
      }
myDFPlayer.play (6);
      for (int i = 190; i >= 35; i--)
      {
        handServo.write(i);
        delay(30);
      }
myDFPlayer.play (6);
      handServo.write(0);
      delay(400);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      delay(1500);
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (6);
      delay(3000);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 7)
    {
      Serial.println("Action 7");      //第七种 关上开关然后摔盒子
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (7);
Serial.print("friend or foe: ");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      delay(400);
      handServo.write(0);
      delay(1500);
      for (int i = 0; i < 6; i++)
      {
        for (int j = 70; j <= 130; j++){
          boxServo.write(j);
          delay(6);
        }
        delay(200);
      }
      delay(500);
myDFPlayer.play (7);
      handServo.write(190);
      delay(900);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 8)
    {
      Serial.println("Action 8");      //第八种 关上开关然后举手抗议
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (8);
Serial.print("real donald trump: ");
Serial.println(digitalRead(PIN_BUSY));
delay (500);
      handServo.write(0);
      delay(900);
      for (int i = 0; i < 7; i++)
      {
        handServo.write(75);
        delay(100);
        handServo.write(40);
        delay(100);
      }
      delay(500);
      handServo.write(190);
      delay(1000);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 9)
    {
      Serial.println("Action 9");      //第九种 露半个头非常生气 慢慢慢慢地关上开关 然后探半个头看
      for (int i = 70; i <= 105; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (9);
Serial.print("fake news: ");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      delay(29);
      boxServo.write(130);
      for (int i = 190; i >=35; i--)
      {
        handServo.write(i);
        delay(40);
      }
      delay(500);
      handServo.write(0);
      delay(200);
      for (int i = 0; i <=190; i++)
      {
        handServo.write(i);
        delay(40);
      }
      boxServo.write(70);
      delay(29);
      boxServo.write(100);
      delay(3000);
      boxServo.write(70);
      action++;
    }
  
    
    else if (action == 10)
    {
      Serial.println("lets do this lets do that");      //第十种 慢慢探半个头看 然后摔盒子 然后迅速关开关
      for (int i = 70; i <= 105; i++)
      {
        boxServo.write(i);
        delay(100);
      }
      delay(1000);
myDFPlayer.play (1);
Serial.print("lets do this: ");
Serial.println(digitalRead(PIN_BUSY));
delay (200);
      for (int i = 0; i < 10; i++)
      {
        for (int j = 70; j <= 100; j++){
          boxServo.write(j);
          delay(6);
      }
        delay(100);
      }
      boxServo.write(130);
      delay(100);
      handServo.write(0);
      delay(850);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 11)
    {
      Serial.println("Action 11");      //第十一种 迅速探头关开关 然后慢速举手抗议 然后再探头按住开挂
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (11);
Serial.print("i dont think so");
Serial.println(digitalRead(PIN_BUSY));
delay (2000);
      handServo.write(0);
      delay(290);
      for (int i = 0; i < 3; i++)
      {
        handServo.write(65);
        delay(200);
        handServo.write(40);
        delay(200);
      }
      delay(150);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      delay(1500);
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
      handServo.write(0);
      delay(300);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 12)
    {
      Serial.println("Action 12");      //第十二种 慢慢地露出头 非常生气 慢慢慢慢地关上开关 然后迅速缩回去
      for (int i = 70; i <= 130; i++)
      {
        boxServo.write(i);
        delay(50);
      }
myDFPlayer.play (12);
Serial.print("if im not right");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      delay(1000);
      for (int i = 190; i >= 0; i--)
      {
        handServo.write(i);
        delay(50);
      }
      delay(29);
      handServo.write(0);
      delay(200);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 13)
    {
      Serial.println("Action 13");      //第十三种 迅速探头 挥手假装要关开关 然后重复几次 关上开关然后缩回去
      for (int i = 70; i <= 130; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (13);
Serial.print("ive been watching you");
Serial.println(digitalRead(PIN_BUSY));
delay (3000);
      delay(29);
      for (int i = 190; i >= 60; i--)
      {
        handServo.write(i);
        delay(40);
      }
      for (int i = 0; i < 3; i++)
      {
        handServo.write(60);
        delay(20);
        handServo.write(40);
        delay(80);
      }
      handServo.write(0);
      delay(990);
      handServo.write(190);
      delay(500);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 14)
    {
      Serial.println("Action 14");     //第十四种 很生气 露出半个头盯着你 然后迅速关掉开关 然后继续盯着你
      for (int i = 70; i <= 105; i++){
        boxServo.write(i);
        delay(6);
      }
myDFPlayer.play (14);
Serial.print("the american dream is dead");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      delay(4000);
      boxServo.write(130);
      handServo.write(0);
      delay(850);
      handServo.write(190);
      delay(500);
      boxServo.write(100);
      delay(4000);
      boxServo.write(70);
      action++;
    }
    
    else if (action == 15)
    {
      Serial.println("Action 15");    //第十五种 很生气 露出半个头然后整个头然后又露剩半个头重复 然后慢慢关掉开关
      for (int i = 70; i <= 105; i++){
        boxServo.write(i);
        delay(6);
      }
      delay(1000);
      for (int i = 0; i < 3; i++)
      {      
        for (int j = 70; j <= 105; j++){
          boxServo.write(j);
          delay(50);
        }
        for (int j = 105; j >= 70; j--)
        {
          boxServo.write(j);
          delay(50);
        }
      }
      for (int j = 70; j <= 130; j++){
        boxServo.write(j);
        delay(50);
      }
myDFPlayer.play (15);
Serial.print("you know beautiful");
Serial.println(digitalRead(PIN_BUSY));
delay (29);
      for (int i = 190; i >= 35; i--)
      {
        handServo.write(i);
        delay(40);
      }
      delay(1000);
      handServo.write(0);
      delay(400);
      handServo.write(190);
      delay(400);
      boxServo.write(70);
      action = 1;
    }
    
  }
  
}
