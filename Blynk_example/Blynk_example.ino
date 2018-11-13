#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#define BLYNK_PRINT  Serial
#define SERIAL_WIFI  Serial3
#include "Blynk_WizFi310.h"

BlynkTimer timer;

char auth[] = "81542f7f559246fcad7d8ddfc28eb6d2";
char ssid[] = "Dap";
char pass[] = "00001111";

float sensorData = 0;

int interval = 1000;
int prevTime = 0;
int currTime = 0;


void sendUptime() {
  Blynk.virtualWrite(V0, sensorData);
  Blynk.virtualWrite(V1, sensorData);
}

void setup() {
  Serial.begin(9600);
  SERIAL_WIFI.begin(115200);

  timer.setInterval(1000, sendUptime);

  WiFi.init(&SERIAL_WIFI);
  Blynk.begin(auth, ssid, pass);

}

void loop() {
  Blynk.run();
  timer.run();

  currTime = millis();
  if (currTime - prevTime >= interval) {

    sensorData++;

    Serial.print("sensorData : ");
    Serial.print(sensorData); // unit: mg/m3
    Serial.println("");
    prevTime = currTime;

  }
}
