#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#define MOTORA_PIN1 12
#define MOTORA_PIN2 13
#define MOTORA_PIN3 8
#define MOTORA_PIN4 9

#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000            //mv

#define BLYNK_PRINT  Serial
#define SERIAL_WIFI  Serial3
#include "Blynk_WizFi310.h"

char auth[] = "81542f7f559246fcad7d8ddfc28eb6d2";
char ssid[] = "Dap";
char pass[] = "00001111";

int Analog_Pin = 1; //Connect dust sensor to Arduino A0 pin
int Digital_Pin = 2;   //Connect 2 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float analogVal = 0;
float digitalVal = 0;
float dustDensity = 0;

int interval = 1000;
int prevTime = 0;
int currTime = 0;

int Calibration(int m)
{
  static int firstFlag = 0, buff[10], sum;
  int buff_max = 10;
  int i;

  if (firstFlag == 0)
  {
    firstFlag = 1;
    for (i = 0, sum = 0; i < buff_max; i++)
    {
      buff[i] = m;
      sum += buff[i];
    }
    return m;
  }
  else
  {
    sum -= buff[0];
    for (i = 0; i < (buff_max - 1); i++)
    {
      buff[i] = buff[i + 1];
    }
    buff[9] = m;
    sum += buff[9];

    i = sum / 10.0;
    return i;
  }
}


void setup() {
  Serial.begin(9600);
  SERIAL_WIFI.begin(115200);

  WiFi.init(&SERIAL_WIFI);
  Blynk.begin(auth, ssid, pass);
  
  pinMode(Digital_Pin, OUTPUT);
  pinMode(MOTORA_PIN1, OUTPUT);
  pinMode(MOTORA_PIN2, OUTPUT);
  pinMode(MOTORA_PIN3, OUTPUT);
  pinMode(MOTORA_PIN4, OUTPUT);
}

void loop() {
  Blynk.run();
  
  currTime = millis();
  if (currTime - prevTime >= interval) {

    digitalWrite(Digital_Pin, LOW); // power on the LED
    delayMicroseconds(samplingTime);
    analogVal = analogRead(Analog_Pin); // read the dust value
    delayMicroseconds(deltaTime);
    digitalWrite(Digital_Pin, HIGH); // turn the LED off
    delayMicroseconds(sleepTime);
    analogVal = Calibration(analogVal);
    digitalVal = analogVal * (SYS_VOLTAGE / 1024.0);

    if (digitalVal >= NO_DUST_VOLTAGE) {
      digitalVal -= NO_DUST_VOLTAGE;
      dustDensity = digitalVal * 0.2; // Voltage(mv)값을  dust density로 변경 (ug/m3)
      dustDensity -= 30;
    } else {
      dustDensity = 0;
    }

    if (dustDensity > 75) {
      Serial.println("dustDensity is higher than 75");
      analogWrite(MOTORA_PIN1, 255);
      digitalWrite(MOTORA_PIN2, LOW);
      analogWrite(MOTORA_PIN3, 255);
      digitalWrite(MOTORA_PIN4, LOW);
    }
    else {
      analogWrite(MOTORA_PIN1, 0);
      digitalWrite(MOTORA_PIN2, LOW);
      analogWrite(MOTORA_PIN3, 0);
      digitalWrite(MOTORA_PIN4, LOW);
    }
    // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
    // Chris Nafis (c) 2012
    Serial.println("**********************************");
    Serial.print(" [Analog Value] : ");
    Serial.println(analogVal);

    Serial.print(" [Voltage] : ");
    Serial.print(digitalVal);
    Serial.println(" mV ");

    Serial.print(" Fine Dust(PM 2.5) : ");
    Serial.print(dustDensity); // unit: mg/m3
    Serial.println(" mg/m3 ");
    Serial.println("**********************************");
    Serial.println("");
    prevTime = currTime;
    Serial.println("curr Time : " + String(currTime));
  }
}
