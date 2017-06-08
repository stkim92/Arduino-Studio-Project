#include <SimpleTimer.h>
#include "Arduino.h"
#include "PIR.h"
#include "SKT_LoRa.h"

#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

SimpleTimer timer;
int PIR_Val;
char send_buf[100];

void reset(){
  PIR_Val = PIR_reCalculate();
  if(PIR_Val== 1){
    //Serial.print("in : "+ String(PIR_Val)); // FOR DEBUG
    sprintf(send_buf, "040104\r\n");
    LoRa.Send(send_buf, 3);
  }
  else{
    //Serial.print("out :"+ String(PIR_Val)); // FOR DEBUG
    sprintf(send_buf, "040100\r\n");
    LoRa.Send(send_buf, 3);
  }

/* FOR DEBUG 
  Serial.println("************wrtie start*************");
  Serial.print("*       send  lightVal : ");
  Serial.print(PIR_Val);
  Serial.println("       *");
  Serial.println("************wrtie finish************");
  Serial.println("");   //to check writing  // to check the lightVal
*/

}

void setup()
{
  
  Serial.begin(38400);
   while (!Serial) {
  }
  //Serial.println("Serial TEST"); // FOR DEBUG
  
  Serial1.begin(38400);
   while (!Serial1) {
  }
  //Serial1.println("Serial1 TEST"); // FOR DEBUG
  
  timer.setInterval(20000,reset);

  PIR_Init();
  LoRa.init(&Serial1);
  LoRa.begin();
 
}

void loop()
{
  if(Serial1.available()){
   Serial.write(Serial1.read());
 }
 timer.run();
}


