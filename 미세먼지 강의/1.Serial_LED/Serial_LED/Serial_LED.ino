void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}
void loop()
{
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == '1')
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Turned on LED ");
    }
    if (ch == '0')
    {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Turned off LED ");
    }
  }
}

