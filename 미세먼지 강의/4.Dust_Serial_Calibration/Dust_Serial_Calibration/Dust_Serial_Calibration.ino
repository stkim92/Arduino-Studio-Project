#if defined(ARDUINO_ARCH_SAMD) // Atmel ARM Cortex core based MCU series
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000            //mv

int Analog_Pin = 1; //Connect dust sensor to Arduino A0 pin
int Digital_Pin = 2;   //Connect 2 led driver pins of dust sensor to Arduino D2

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float analogVal = 0;
float digitalVal = 0;
float dustDensity = 0;

int Calibration(int m)
{
  static int firstFlag = 0, buff[10], sum;
  int buff_max = 10;
  int i;
  
  if(firstFlag == 0)
  {
    firstFlag = 1;

    for(i = 0, sum = 0; i < buff_max; i++)
    {
      buff[i] = m;
      sum += buff[i];
    }
    return m;
  }
  else
  {
    sum -= buff[0];
    for(i = 0; i < (buff_max - 1); i++)
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
  pinMode(Digital_Pin, OUTPUT);
}

void loop() {
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

  delay(1000);
}
