#include <Arduino.h>
#include <CodeBaseClass.h>



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
    if (adc <= 45)
        return Block_RedLED_ON;      // 100Ω → ADC ~7

    if (adc <= 125)
        return Block_YellowLED_ON;   // 1kΩ → ADC ~90

    if (adc <= 260)
        return Block_GreenLED_ON;    // 2kΩ → ADC ~168


    // OFF blocks
    if (adc <= 450)
        return Block_RedLED_OFF;     // 5.7kΩ → ADC ~371

    if (adc <= 575)
        return Block_YellowLED_OFF;  // 11.5kΩ → ADC ~543

    if (adc <= 640)
        return Block_GreenLED_OFF;   // 14.7kΩ → ADC ~608


    // Delay blocks
    if (adc <= 725)
        return Block_Delay1s;        // 20kΩ → ADC ~680

    if (adc <= 835)
        return Block_Delay2s;        // 32kΩ → ADC ~775

    if (adc <= 935)
        return Block_Delay3s;        // 69kΩ → ADC ~898


    if(adc>=1000)
    return Block_Empty;

    return Block_Empty;              // No block
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