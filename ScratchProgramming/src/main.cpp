#include <Arduino.h>
#include <CodeBaseClass.h>
#include <CodeLEDClass.h>

void setup()
{
  Serial.begin(9600);



  pinMode(Green_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Red_LED, OUTPUT);

  delay(300);
}

 CodeLED codeLED;

void loop()
{
  codeLED.addBlocks();
  codeLED.writeBlocks();
  codeLED.executeBlocks();

  Serial.println("\nDebug:");

  Serial.print("ADC1 = "); Serial.println(readStableADC(pin1));
  Serial.print("ADC2 = "); Serial.println(readStableADC(pin2));
  Serial.print("ADC3 = "); Serial.println(readStableADC(pin3));
  Serial.print("ADC4 = "); Serial.println(readStableADC(pin4));
  Serial.print("ADC5 = "); Serial.println(readStableADC(pin5));
  Serial.print("ADC6 = "); Serial.println(readStableADC(pin6));

  Serial.println("---------------------------");

  delay(1000);

}
