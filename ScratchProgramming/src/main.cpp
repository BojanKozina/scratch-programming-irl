#include <Arduino.h>
#include <CodeBaseClass.h>
#include <CodeLEDClass.h>

const uint8_t BUTTON_PIN = 13;
const uint8_t MAX_SEQUENCE_LENGTH = 9;

enum SimonState
{
  Simon_WaitToStart,
  Simon_WaitToShow,
  Simon_WaitToCheck
};

CodeLED codeLED;
codeBlockLED simonSequence[MAX_SEQUENCE_LENGTH];
uint8_t sequenceLength = 1;
SimonState simonState = Simon_WaitToStart;

bool buttonPressed()
{
  if (digitalRead(BUTTON_PIN) != LOW)
    return false;

  delay(25);

  if (digitalRead(BUTTON_PIN) != LOW)
    return false;

  while (digitalRead(BUTTON_PIN) == LOW)
  {
  }

  delay(25);
  return true;
}

codeBlockLED randomSimonBlock()
{
  switch (random(0, 9))
  {
  case 0:
    return Block_RedLED_ON;
  case 1:
    return Block_YellowLED_ON;
  case 2:
    return Block_GreenLED_ON;
  case 3:
    return Block_RedLED_Blink2;
  case 4:
    return Block_YellowLED_Blink2;
  case 5:
    return Block_GreenLED_Blink2;
  case 6:
    return Block_RedLED_Blink3;
  case 7:
    return Block_YellowLED_Blink3;
  default:
    return Block_GreenLED_Blink3;
  }
}

codeBlockLED randomSimonBlockForRound(uint8_t roundNumber)
{
  if (roundNumber <= 3)
    return static_cast<codeBlockLED>(random(0, 3));

  if (roundNumber <= 6)
    return static_cast<codeBlockLED>(random(0, 6));

  return randomSimonBlock();
}

void turnAllOutputsOff()
{
  digitalWrite(Red_LED, LOW);
  digitalWrite(Yellow_LED, LOW);
  digitalWrite(Green_LED, LOW);
  noTone(Buzzer_PIN); // digitalWrite(Buzzer_PIN, LOW);
}

void pulseOutput(uint8_t pin, uint16_t pulseMs)
{
  digitalWrite(pin, HIGH);
  // digitalWrite(Buzzer_PIN, HIGH);
  delay(pulseMs);
  turnAllOutputsOff();
}

void showSimonBlock(codeBlockLED block)
{
  switch (block)
  {
  case Block_RedLED_ON:
    tone(Buzzer_PIN, 440); pulseOutput(Red_LED, 350);
    break;
  case Block_YellowLED_ON:
    tone(Buzzer_PIN, 523); pulseOutput(Yellow_LED, 350);
    break;
  case Block_GreenLED_ON:
    tone(Buzzer_PIN, 659); pulseOutput(Green_LED, 350);
    break;

  case Block_RedLED_Blink2:
  case Block_YellowLED_Blink2:
  case Block_GreenLED_Blink2:
  case Block_RedLED_Blink3:
  case Block_YellowLED_Blink3:
  case Block_GreenLED_Blink3:
    codeLED.executeCodeFromCodeBlock(block);
    break;

  default:
    break;
  }

  turnAllOutputsOff();
  delay(180);
}

void showSequence() {
  Serial.print("Simon says round ");
  Serial.println(sequenceLength);
  for (uint8_t i = 0; i < sequenceLength; i++) {
    showSimonBlock(simonSequence[i]);
  }
  Serial.println("Repeat the sequence with the blocks, then press the button to check it.");
}

void showPlayerBlocks() {
  codeLED.addBlocks();
  Serial.println("Blocks detected:");
  codeLED.writeBlocks();
}

void successFlash()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Yellow_LED, HIGH);
    digitalWrite(Green_LED, HIGH);
    digitalWrite(Buzzer_PIN, HIGH);
    delay(120);

    digitalWrite(Red_LED, LOW);
    digitalWrite(Yellow_LED, LOW);
    digitalWrite(Green_LED, LOW);
    digitalWrite(Buzzer_PIN, LOW);
    delay(120);
  }
}

void failureFlash()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    digitalWrite(Red_LED, HIGH);
    digitalWrite(Buzzer_PIN, HIGH);
    delay(180);
    digitalWrite(Red_LED, LOW);
    digitalWrite(Buzzer_PIN, LOW);
    delay(120);
  }
}

void replayPlayerBlock(codeBlockLED block)
{
  switch (block)
  {
  case Block_RedLED_ON:
    tone(Buzzer_PIN, 440); pulseOutput(Red_LED, 350);
    break;
  case Block_YellowLED_ON:
    tone(Buzzer_PIN, 523); pulseOutput(Yellow_LED, 350);
    break;
  case Block_GreenLED_ON:
    tone(Buzzer_PIN, 659); pulseOutput(Green_LED, 350);
    break;

  case Block_RedLED_Blink2:
  case Block_YellowLED_Blink2:
  case Block_GreenLED_Blink2:
  case Block_RedLED_Blink3:
  case Block_YellowLED_Blink3:
  case Block_GreenLED_Blink3:
    codeLED.executeCodeFromCodeBlock(block);
    break;

  default:
    break;
  }

  turnAllOutputsOff();
}

void replayPlayerSequence()
{
  for (uint8_t i = 0; i < codeLED.getCurrentActiveBlocks(); i++)
  {
    turnAllOutputsOff();
    delay(160);
    replayPlayerBlock(codeLED.getBlockAt(i));
    delay(180);
  }

  turnAllOutputsOff();
}

bool matchesSequence()
{
  if (codeLED.getCurrentActiveBlocks() != sequenceLength)
    return false;

  for (uint8_t i = 0; i < sequenceLength; i++)
  {
    if (codeLED.getBlockAt(i) != simonSequence[i])
      return false;
  }

  return true;
}

bool checkPlayerSequence()
{
  codeLED.addBlocks();

  Serial.println("You placed:");
  codeLED.writeBlocks();

  Serial.println("Replaying your blocks:");
  replayPlayerSequence();

  delay(1200);

  return matchesSequence();
}

void setup()
{
  Serial.begin(9600);

  initMultiplexerPins();

  pinMode(Green_LED, OUTPUT);
  pinMode(Yellow_LED, OUTPUT);
  pinMode(Red_LED, OUTPUT);
  pinMode(Buzzer_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(Buzzer_PIN, LOW);
  digitalWrite(Red_LED, LOW);
  digitalWrite(Yellow_LED, LOW);
  digitalWrite(Green_LED, LOW);
  turnAllOutputsOff();

  randomSeed(analogRead(A5) ^ micros());

  simonSequence[0] = randomSimonBlockForRound(sequenceLength);

  delay(300);
  Serial.println("Press the button on pin 13 to start Simon Says.");
}

void loop()
{
  if (simonState == Simon_WaitToStart)
  {
    if (buttonPressed())
    {
      Serial.print("Round length: ");
      Serial.println(sequenceLength);

      showSequence();
      simonState = Simon_WaitToCheck;
    }
  }
  else if (simonState == Simon_WaitToCheck)
  {
    if (!buttonPressed())
      return;

    if (checkPlayerSequence())
    {
      Serial.println("Correct!");
      successFlash();

      if (sequenceLength < MAX_SEQUENCE_LENGTH)
      {
        simonSequence[sequenceLength] = randomSimonBlockForRound(sequenceLength + 1);
        sequenceLength++;
      }

      simonState = Simon_WaitToStart;
    }
    else
    {
      Serial.println("Wrong sequence. Restarting.");
      failureFlash();

      sequenceLength = 1;
      for (uint8_t i = 0; i < MAX_SEQUENCE_LENGTH; i++)
        simonSequence[i] = Block_Empty;

      simonSequence[0] = randomSimonBlockForRound(1);
      simonState = Simon_WaitToStart;
    }
  }

}



