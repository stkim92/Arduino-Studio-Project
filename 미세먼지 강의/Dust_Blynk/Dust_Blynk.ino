#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif
#define BLYNK_PRINT  Serial
/* Defines the serial connected to WizFi310 */
#define SERIAL_WIFI  Serial3

#define MOTORA_PIN1 12
#define MOTORA_PIN2 13
#define MOTORA_PIN3 8
#define MOTORA_PIN4 9

#include "Blynk_WizFi310.h"

char auth[] = "70040b75f5e94babb76cd4ad3ad94cd3";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dap";
char pass[] = "00001111";

int measurePin = 1; //Connect dust sensor to Arduino A0 pin
int ledPower = 2;   //Connect 3 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0; 
float dustDensity = 0;

int criteria = 0;
BlynkTimer timer;


BLYNK_WRITE(V4) {
  int val = param.asInt();
  criteria = val;
  delay(500);
  Blynk.virtualWrite(V3, criteria);
  delay(500);
  Blynk.virtualWrite(V5, criteria);
}

BLYNK_WRITE(V5) {
  int val = param.asInt();
  criteria = val;
  delay(500);
  Blynk.virtualWrite(V3, criteria);
  delay(500);
  Blynk.virtualWrite(V4, criteria);
}

void sendUptime() {
  Blynk.virtualWrite(V1, voMeasured);
  Blynk.virtualWrite(V2, voMeasured);
}

void setup() {
  Serial.begin(9600);
  SERIAL_WIFI.begin(115200);

  WiFi.init(&SERIAL_WIFI);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000, sendUptime);

  pinMode(ledPower, OUTPUT);
  pinMode(MOTORA_PIN1, OUTPUT); //analogWrite시 구지 핀모드를 출력으로 설정할필요 없지만, 확실히 하기 위해서 출력으로 설정했습니다.
  pinMode(MOTORA_PIN2, OUTPUT);    //analogWrite시 구지 핀모드를 출력으로 설정할필요 없지만, 확실히 하기 위해서 출력으로 설정했습니다.
  pinMode(MOTORA_PIN3, OUTPUT); //analogWrite시 구지 핀모드를 출력으로 설정할필요 없지만, 확실히 하기 위해서 출력으로 설정했습니다.
  pinMode(MOTORA_PIN4 , OUTPUT);


}


void loop() {
  Blynk.run();
  timer.run();
  digitalWrite(ledPower, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value





  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  ////////calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  //////////dustDensity = 0.17 * calcVoltage - 0.1;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.println(voMeasured);
  if (voMeasured > criteria) {
    Serial.println("more than 300!!!!!!!!!!!!!!!!!!!!!");
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


  ////////////Serial.print(" - Voltage: ");
  ///////////Serial.print(calcVoltage);

  ////////////// Serial.print(" - Dust Density: ");
  ///////////// Serial.println(dustDensity); // unit: mg/m3

  delay(1000);
}
