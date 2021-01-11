#include "parser.h"

// Command directives
#define CMD_HELP "help"
#define CMD_TX "tx"
#define CMD_FREQ "freq"
#define CMD_SEND "send"

// Data buffers
#define BUFFER_SIZE 64
char conversionBuffer[BUFFER_SIZE];
uint8_t sendBuffer[BUFFER_SIZE];

void parseCommand(RH_RF95 &rf95, String serialData)
{
    // Determine the command root and call the appropriate function.
    // There's probably a cleaner way to do this but I am not a C++ programmer.
    if (serialData.substring(0, String(CMD_HELP).length()) == CMD_HELP)
    {
        cmdHelp();
    }
    else if (serialData.substring(0, String(CMD_TX).length()) == CMD_TX)
    {
        serialData.remove(0, String(CMD_TX).length() + 1); // Consolidate passed argument by removing command.
        cmdTx(rf95, serialData);
    }
    else if (serialData.substring(0, String(CMD_FREQ).length()) == CMD_FREQ)
    {
        serialData.remove(0, String(CMD_FREQ).length() + 1); // Consolidate passed argument by removing command.
        cmdFreq(rf95, serialData);
    }
    else if (serialData.substring(0, String(CMD_SEND).length()) == CMD_SEND)
    {
        serialData.remove(0, String(CMD_SEND).length() + 1); // Consolidate passed argument by removing command.
        cmdSend(rf95, serialData);
    }
}

void cmdHelp()
{
    Serial.println("LoRa SDCS\tWritten by Will S.");
    Serial.println("help\tRetrieve a list of commands and their functions.");
    Serial.println("tx dBm\tSet transmitter dBm. Valid values are from +2 to +20.");
    Serial.println("freq MHz\tSet transmitter frequency in Mhz. Valid values are from 137.0 to 1020.0.");
    Serial.println("send message\tTransmit message through LoRa module.");
    return;
}

void cmdTx(RH_RF95 &rf95, String serialData)
{
    int txLevel;
    txLevel = serialData.toInt(); // Convert argument to integer. Defaults to 0 if string cannot be converted.
    if (!serialData.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify a power output level in dBm. (+2 to +20)");
        return;
    }
    else if (txLevel < 2 || txLevel > 20)
    {
        // ERROR: Invalid output level has been given.
        Serial.println("Power output level is invalid! Please try again.");
        return;
    }
    else
    {
        // Valid output level has been detected.
        rf95.setTxPower(txLevel, false);
        Serial.println("Power output level: " + String(txLevel) + "dBm");
        return;
    }
}

void cmdFreq(RH_RF95 &rf95, String serialData)
{
    float freq;
    freq = serialData.toFloat(); // Convert argument to float. Defaults to 0 if string cannot be converted.
    if (!serialData.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify a frequency in MHz. (137.0 MHz to 1020.0 MHz)");
        return;
    }
    else if (freq < 137.0 || freq > 1020.0)
    {
        // ERROR: Invalid output level has been given.
        Serial.println("Frequency is invalid! Please try again.");
        return;
    }
    else
    {
        // Valid frequency has been detected.
        rf95.setFrequency(freq);
        Serial.println("Frequency (MHz): " + String(freq) + "MHz");
        return;
    }
}

void cmdSend(RH_RF95 &rf95, String serialData)
{
    serialData.toCharArray(conversionBuffer, BUFFER_SIZE); // Convert string to a char array.
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        // Iterate through the conversionBuffer and cast each char to uint8_t.
        // uint8_t is the datatype expected by the RadioHead library.
        sendBuffer[i] = uint8_t(conversionBuffer[i]);
    }
    if (!serialData.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify data to be sent.");
        return;
    }
    else
    {
        // Valid frequency has been detected.
        rf95.send(sendBuffer, sizeof(sendBuffer)); // Send the converted buffer
        Serial.println("TX> " + serialData);       // Echo the sent data back to serial
        rf95.waitPacketSent();                     // Wait for transmission to finish before returning.
        return;
    }
}