#include <Arduino.h>
#include <CodeBaseClass.h>
#include <CodeLEDClass.h>

void setup()
{
  Serial.begin(9600);

  initMultiplexerPins();

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
  Serial.print("ADC6 (MUX CH0) = "); Serial.println(readMultiplexerStableADC(0));
  Serial.print("ADC7 (MUX CH1) = "); Serial.println(readMultiplexerStableADC(1));
  Serial.print("ADC8 (MUX CH2) = "); Serial.println(readMultiplexerStableADC(2));
  Serial.print("ADC9 (MUX CH3) = "); Serial.println(readMultiplexerStableADC(3));

  Serial.println("---------------------------");

  delay(6000);

}
