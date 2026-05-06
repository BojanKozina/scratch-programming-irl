#include <Arduino.h>
#include <CodeBaseClass.h>



// Digital pins for turning on/off the leds
const uint8_t Red_LED = 9;
const uint8_t Yellow_LED = 10;
const uint8_t Green_LED = 11;
const uint8_t Buzzer_PIN = 12;

enum codeBlockLED
{
    // Led on blocks
    Block_RedLED_ON,
    Block_YellowLED_ON,
    Block_GreenLED_ON,

    // 2-blink blocks
    Block_RedLED_Blink2,
    Block_YellowLED_Blink2,
    Block_GreenLED_Blink2,

    // 3-blink blocks
    Block_RedLED_Blink3,
    Block_YellowLED_Blink3,
    Block_GreenLED_Blink3,

    Block_Empty
};

class CodeLED : public CodeBase<codeBlockLED, Block_Empty, 9>
{
public:
    void blinkOutput(uint8_t pin, uint8_t times, uint16_t frequency)
    {
        for (uint8_t i = 0; i < times; i++)
        {
            tone(Buzzer_PIN, frequency);
            digitalWrite(pin, HIGH);
            delay(85);
            digitalWrite(pin, LOW);
            noTone(Buzzer_PIN);

            if (i + 1 < times)
                delay(55);
        }
    }

    void blinkAllOutputs(uint8_t times)
    {
        for (uint8_t i = 0; i < times; i++)
        {
            digitalWrite(Red_LED, HIGH);
            digitalWrite(Yellow_LED, HIGH);
            digitalWrite(Green_LED, HIGH);
            digitalWrite(Buzzer_PIN, HIGH);
            delay(250);

            digitalWrite(Red_LED, LOW);
            digitalWrite(Yellow_LED, LOW);
            digitalWrite(Green_LED, LOW);
            digitalWrite(Buzzer_PIN, LOW);

            if (i + 1 < times)
                delay(250);
        }
    }

    String codeBlockToString(codeBlockLED codeBlock) const override
    {
        switch (codeBlock)
        {
        // ON Blocks
        case Block_RedLED_ON:
            return "Block_RedLED_ON";
        case Block_YellowLED_ON:
            return "Block_YellowLED_ON";
        case Block_GreenLED_ON:
            return "Block_GreenLED_ON";

        // 2-blink Blocks
        case Block_RedLED_Blink2:
            return "Block_RedLED_Blink2";
        case Block_YellowLED_Blink2:
            return "Block_YellowLED_Blink2";
        case Block_GreenLED_Blink2:
            return "Block_GreenLED_Blink2";

        // 3-blink Blocks
        case Block_RedLED_Blink3:
            return "Block_RedLED_Blink3";
        case Block_YellowLED_Blink3:
            return "Block_YellowLED_Blink3";
        case Block_GreenLED_Blink3:
            return "Block_GreenLED_Blink3";

        default:
            return "Block_Empty";
        }
    }

codeBlockLED returnCodeBlockType(int adc) const override
{
    // ON blocks
    if (adc <= 45)
        return Block_RedLED_ON;      // 100Ω → ADC ~7

    if (adc <= 125)
        return Block_YellowLED_ON;   // 1kΩ → ADC ~90

    if (adc <= 260)
        return Block_GreenLED_ON;    // 2kΩ → ADC ~168


    // 2-blink blocks
    if (adc <= 450)
        return Block_RedLED_Blink2;     // 5.7kΩ → ADC ~371

    if (adc <= 575)
        return Block_YellowLED_Blink2;  // 11.5kΩ → ADC ~543

    if (adc <= 640)
        return Block_GreenLED_Blink2;   // 14.7kΩ → ADC ~608


    // 3-blink blocks
    if (adc <= 725)
        return Block_RedLED_Blink3;        // 20kΩ → ADC ~680

    if (adc <= 835)
        return Block_YellowLED_Blink3;        // 32kΩ → ADC ~775

    if (adc <= 935)
        return Block_GreenLED_Blink3;        // 69kΩ → ADC ~898


    if(adc>=1000)
    return Block_Empty;

    return Block_Empty;              // No block
}


    void addBlocks() override
    {

        clearBlocks();

        digitalWrite(Red_LED, LOW);
        digitalWrite(Yellow_LED, LOW);
        digitalWrite(Green_LED, LOW);

        addBlock(pin1);
        addBlock(pin2);
        addBlock(pin3);
        addBlock(pin4);
        addBlock(pin5);
        addBlockFromMultiplexer(0); // block6
        addBlockFromMultiplexer(1); // block7
        addBlockFromMultiplexer(2); // block8
        addBlockFromMultiplexer(3); // block9
    }

    void executeCodeFromCodeBlock(codeBlockLED codeBlock) override
    {
        switch (codeBlock)
        {
        case Block_RedLED_ON:
            digitalWrite(Red_LED, HIGH);
            break;
        case Block_YellowLED_ON:
            digitalWrite(Yellow_LED, HIGH);
            break;
        case Block_GreenLED_ON:
            digitalWrite(Green_LED, HIGH);
            break;

        case Block_RedLED_Blink2:
            blinkOutput(Red_LED, 2, 440);
            break;
        case Block_YellowLED_Blink2:
            blinkOutput(Yellow_LED, 2, 523);
            break;
        case Block_GreenLED_Blink2:
            blinkOutput(Green_LED, 2, 659);
            break;

        case Block_RedLED_Blink3:
            blinkOutput(Red_LED, 3, 440);
            break;
        case Block_YellowLED_Blink3:
            blinkOutput(Yellow_LED, 3, 523);
            break;
        case Block_GreenLED_Blink3:
            blinkOutput(Green_LED, 3, 659);
            break;

        default:
            break;
        }
    }
};
