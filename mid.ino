#include <Stepper.h> //引入步進馬達函示庫
#include "pitches.h"

const int stepsPerRevolution = 2048;  //此馬達轉一圈為2048步
const int RPM = 5; //馬達旋轉速度
int freq = 2000;
int channel = 0;
int resolution = 8;
int tonePin = 16;
//宣告Stepper物件，的步數和引腳
//引角依序對應驅動版的1N1、1N3、1N2、1N4
Stepper myStepper(stepsPerRevolution, 13, 14, 12, 27);  

 int CanonTone[] = {  
   NOTE_E3, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_B2, NOTE_G4, NOTE_B4, NOTE_D5,  
   NOTE_C3, NOTE_E4, NOTE_A4, NOTE_C5, NOTE_G2, NOTE_E4, NOTE_G4, NOTE_B4,  
   NOTE_A2, NOTE_C4, NOTE_F4, NOTE_A4, NOTE_E1, NOTE_C4, NOTE_E4, NOTE_G4,  
  NOTE_A2, NOTE_D4, NOTE_F4, NOTE_A4, NOTE_B2, NOTE_D4, NOTE_G4, NOTE_B4,  
   NOTE_E3, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5,   
   NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_G4,   
   NOTE_F4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4,   
  NOTE_F4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5,  
   NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,  
   NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_C5,  
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,  
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_B4,  
  NOTE_C5,0
  };

  int noteDurations[] = {  
 8, 8, 8, 8, 8, 8, 8, 8,  
  8, 8, 8, 8, 8, 8, 8, 8,   
 8, 8, 8, 8, 8, 8, 8, 8,   
 8, 8, 8, 8, 8, 8, 8, 8,   
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  8,16,16,8,16,16,16,16,16,16,16,16,16, 16,  
  };   

  unsigned long currentTime = 0, tonePreviousTime = 0, ooPreviousTime = 0;

  int pauseBetweenNotes = 0;
int noteDuration = 0;
int thisNote = 0;
boolean nextTone = true;
boolean nextoo = true;

void setup() {  
  Serial.begin(115200);
  myStepper.setSpeed(RPM);  //設定馬達轉速
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(tonePin, channel);
  pinMode(18,INPUT);
}

void loop() {
  

  if(digitalRead(18)==HIGH) {
       if(nextTone){
       int noteDuration = 1000 / noteDurations[thisNote];
       ledcWriteTone(channel, CanonTone[thisNote]);
       pauseBetweenNotes = noteDuration * 1.30;
       nextTone = false;
       }


      if(nextoo){
        myStepper.step(50);
        nextoo = false;
      }
 
     currentTime = millis(); //記錄現在時間
     if((currentTime - ooPreviousTime) > 10 ){
   
       nextoo = true;
       ooPreviousTime = currentTime;
       } 
     if((currentTime - tonePreviousTime) > noteDuration){
      ledcWriteTone(channel, 0);
       } //buzzer停止播放
     if((currentTime - tonePreviousTime) > (pauseBetweenNotes)){
      nextTone = true;
      thisNote++;
      if(thisNote == 144) thisNote = 0;
      tonePreviousTime = currentTime; 
      } //buzzer播放下一個音
   
   }
   
}
