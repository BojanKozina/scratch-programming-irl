#pragma once
#include <Arduino.h>

// Gives a way less noisy output for the adc instead of analogRead
inline int readStableADC(uint8_t pin)
{
    int samples = 100;

    long sum = 0;
    for (int i = 0; i < samples; i++)
        sum += analogRead(pin);

    return sum / samples;
}

template <class codeBlockTemplate, codeBlockTemplate EmptyBlock, uint8_t maxBlocks>
class CodeBase
{
    codeBlockTemplate codeBlocks[maxBlocks];
    uint8_t currentActiveBlocks{0};

public:
    CodeBase()
    {
        for (uint8_t i = 0; i < maxBlocks; i++)
            codeBlocks[i] = EmptyBlock;
    }

    virtual String codeBlockToString(codeBlockTemplate codeBlock) const = 0;
    virtual codeBlockTemplate returnCodeBlockType(int adc) const = 0;
    virtual void executeCodeFromCodeBlock(codeBlockTemplate codeBlock) = 0;
    virtual void addBlocks() = 0;

    // For adding a single block inside the code
    void addBlock(uint8_t pin)
    {
        if (currentActiveBlocks >= maxBlocks)
            return;

        int adc = readStableADC(pin);
        codeBlockTemplate cb = returnCodeBlockType(adc);

        if (cb != EmptyBlock)
        {
            codeBlocks[currentActiveBlocks] = cb;
            currentActiveBlocks++;
        }
    }

    void clearBlocks()
    {
        currentActiveBlocks = 0;
    }

    // Method that runs the final code.
    void executeBlocks()
    {
        for (uint8_t i = 0; i < currentActiveBlocks; i++)
            executeCodeFromCodeBlock(codeBlocks[i]);
    }

    void writeBlocks()
    {
        Serial.println();
        for (uint8_t i = 0; i < maxBlocks; i++)
        {
            Serial.print("Block [");
            Serial.print(i + 1);
            Serial.print("] = ");
            Serial.println(codeBlockToString(codeBlocks[i]));
        }

        Serial.print("Active Blocks: ");
        Serial.println(currentActiveBlocks);
    }

    virtual ~CodeBase() = default;
};