#include "Arduino.h"
#include "PIR.h"

void PIR_Init(){
  pinMode(PIR_Sensor,INPUT);
}

int PIR_Sensing(int _pirPin){
  int PIR_Val = 0;
  PIR_Val = digitalRead(_pirPin);
  return PIR_Val;
}

int PIR_reCalculate(){
  int PIR_Sum =0;
  for(int i=0; i<10; i++){
    PIR_Sum += PIR_Sensing(PIR_Sensor);
    //Serial.println("PIR_Val : "+String(PIR_Val)+"\r\n"); // FOR DEBUG
    delay(1000);
  }
  if(PIR_Sum > 5){
    //Serial.println("PIR_Sum : "+String(PIR_Sum)+"\r\n"); // FOR DEBUG
    return 1;
  }
  else{
    //Serial.println("PIR_Sum : "+String(PIR_Sum)+"\r\n"); // FOR DEBUG
    return 0;
  }
}


