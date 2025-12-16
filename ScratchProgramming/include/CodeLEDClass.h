#include <Arduino.h>
#include <CodeBaseClass.h>

// Pins, blocks, pins 7-9 will be on a multiplexer for extra pins
const uint8_t pin1 = A5;
const uint8_t pin2 = A4;
const uint8_t pin3 = A3;
const uint8_t pin4 = A2;
const uint8_t pin5 = A1;
const uint8_t pin6 = A0;

// const uint8_t pin7 = x;
// const uint8_t pin8 = y;
// const uint8_t pin9 = z;

// Digital pins for turning on/off the leds
const uint8_t Red_LED = 2;
const uint8_t Yellow_LED = 3;
const uint8_t Green_LED = 4;

enum codeBlockLED
{
    // Led on blocks
    Block_RedLED_ON,
    Block_YellowLED_ON,
    Block_GreenLED_ON,

    // Led off blocks
    Block_RedLED_OFF,
    Block_YellowLED_OFF,
    Block_GreenLED_OFF,

    // Delay blocks
    Block_Delay1s,
    Block_Delay2s,
    Block_Delay3s,

    Block_Empty
};

class CodeLED : public CodeBase<codeBlockLED, Block_Empty, 9>
{
public:
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

        // OFF Blocks
        case Block_RedLED_OFF:
            return "Block_RedLED_OFF";
        case Block_YellowLED_OFF:
            return "Block_YellowLED_OFF";
        case Block_GreenLED_OFF:
            return "Block_GreenLED_OFF";

        // Delay Blocks
        case Block_Delay1s:
            return "Block_Delay1s";
        case Block_Delay2s:
            return "Block_Delay2s";
        case Block_Delay3s:
            return "Block_Delay3s";

        default:
            return "Block_Empty";
        }
    }

codeBlockLED returnCodeBlockType(int adc) const override
{
    // ON blocks
    if (adc <= 105)
        return Block_RedLED_ON;

    if (adc <= 200)
        return Block_YellowLED_ON;

    if (adc <= 258)
        return Block_GreenLED_ON;


    // OFF blocks
    if (adc <= 340)
        return Block_RedLED_OFF;

    if (adc <= 380)
        return Block_YellowLED_OFF;

    if (adc <= 420)
        return Block_GreenLED_OFF;


    // Delay blocks
    if (adc <= 455)
        return Block_Delay1s;

    if (adc <= 490)
        return Block_Delay2s;

    if (adc <= 510)
        return Block_Delay3s;


    return Block_Empty;
}


    void addBlocks() override
    {

        clearBlocks();

        addBlock(pin1);
        addBlock(pin2);
        addBlock(pin3);
        addBlock(pin4);
        addBlock(pin5);
        addBlock(pin6);
        // addBlock(pin7);
        // addBlock(pin8);
        // addBlock(pin9);
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

        case Block_RedLED_OFF:
            digitalWrite(Red_LED, LOW);
            break;
        case Block_YellowLED_OFF:
            digitalWrite(Yellow_LED, LOW);
            break;
        case Block_GreenLED_OFF:
            digitalWrite(Green_LED, LOW);
            break;

        case Block_Delay1s:
            delay(1000);
            break;
        case Block_Delay2s:
            delay(2000);
            break;
        case Block_Delay3s:
            delay(3000);
            break;

        default:
            break;
        }
    }
};