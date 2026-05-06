#pragma once
#include <Arduino.h>

// Direct ADC block pins.
// UNO has only A0-A5, so A0 is reserved as multiplexer SIG.
const uint8_t pin1 = A5;
const uint8_t pin2 = A4;
const uint8_t pin3 = A3;
const uint8_t pin4 = A2;
const uint8_t pin5 = A1;
// Block 6-9 are read from multiplexer channels.

// 16-channel multiplexer (74HC4067 compatible) pins
const uint8_t MUX_S0_PIN = 5;
const uint8_t MUX_S1_PIN = 6;
const uint8_t MUX_S2_PIN = 7;
const uint8_t MUX_S3_PIN = 8;
const uint8_t MUX_SIG_PIN = A0;

inline int readStableADC(uint8_t pin);


inline void initMultiplexerPins()
{
    pinMode(MUX_S0_PIN, OUTPUT);
    pinMode(MUX_S1_PIN, OUTPUT);
    pinMode(MUX_S2_PIN, OUTPUT);
    pinMode(MUX_S3_PIN, OUTPUT);
}

inline void selectMultiplexerChannel(uint8_t channel)
{
    digitalWrite(MUX_S0_PIN, channel & 0x01);
    digitalWrite(MUX_S1_PIN, (channel >> 1) & 0x01);
    digitalWrite(MUX_S2_PIN, (channel >> 2) & 0x01);
    digitalWrite(MUX_S3_PIN, (channel >> 3) & 0x01);
}

inline int readMultiplexerStableADC(uint8_t channel)
{
    selectMultiplexerChannel(channel);
    delayMicroseconds(50);
    return readStableADC(MUX_SIG_PIN);
}


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

    void addBlockFromMultiplexer(uint8_t channel)
    {
        if (currentActiveBlocks >= maxBlocks)
            return;

        int adc = readMultiplexerStableADC(channel);
        codeBlockTemplate cb = returnCodeBlockType(adc);

        if (cb != EmptyBlock)
        {
            codeBlocks[currentActiveBlocks] = cb;
            currentActiveBlocks++;
        }
    }

    void clearBlocks()
    {
        for (uint8_t i = 0; i < maxBlocks; i++)
            codeBlocks[i] = EmptyBlock;

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

    uint8_t getCurrentActiveBlocks() const
    {
        return currentActiveBlocks;
    }

    codeBlockTemplate getBlockAt(uint8_t index) const
    {
        if (index >= currentActiveBlocks)
            return EmptyBlock;

        return codeBlocks[index];
    }

    virtual ~CodeBase() = default;
}; 