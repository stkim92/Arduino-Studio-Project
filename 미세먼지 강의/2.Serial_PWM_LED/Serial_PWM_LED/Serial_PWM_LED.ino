int Value = 0;
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == '8')
    {
      Value += 30;
      if ( Value > 255 || Value < 0 ) {
        Serial.println("Value : "+String(Value));
        Serial.println("Value is not in 0 ~255. ");
      }
      else {
        Serial.println("Value : " + String(Value));
        analogWrite(LED_BUILTIN, Value);
      }
    }
    if (ch == '2')
    {
      Value -= 30;
      if ( Value > 255 || Value < 0 ) {
        Serial.println("Value : "+ String(Value));
        Serial.println("Value is not in 0 ~255. ");
      }
      else {
        Serial.println("Value : "+String(Value));
        analogWrite(LED_BUILTIN, Value);
      }
    }
  }
}

