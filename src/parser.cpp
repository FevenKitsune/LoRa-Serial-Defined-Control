#include "parser.h"

// Command directives
#define CMD_HELP "help"
#define CMD_TX "tx"
#define CMD_FREQ "freq"
#define CMD_SEND "send"

// Data buffers
#define BUFFER_SIZE 64

void parseCommand(RH_RF95 &rf95, String &serialData)
{
    String command = popArg(serialData);
    String arg = popArg(serialData);

    /*
    Serial.println("DEBUG:");
    Serial.println("command=" + command);
    Serial.println("arg=" + arg);
    Serial.println("serialData=" + serialData);
    */

    // Determine the command root and call the appropriate function.
    // There's probably a cleaner way to do this but I am not a C++ programmer.
    if (command == CMD_HELP)
    {
        cmdHelp();
    }
    else if (command == CMD_TX)
    {
        cmdTx(rf95, arg);
    }
    else if (command == CMD_FREQ)
    {
        cmdFreq(rf95, arg);
    }
    else if (command == CMD_SEND)
    {
        cmdSend(rf95, arg);
    }
}

String popArg(String &data)
{
    String toReturn = data.substring(0, data.indexOf(' '));
    data.remove(0, data.indexOf(' ') + 1 ? data.indexOf(' ') + 1 : data.length());
    // Clears string up to next space. data.indexOf(' ') will return -1 if no space is found.
    // Therefore, data.indexOf(' ') + 1 == 0 or False
    return toReturn;
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

void cmdTx(RH_RF95 &rf95, String &arg)
{
    int txLevel;
    txLevel = arg.toInt(); // Convert argument to integer. Defaults to 0 if string cannot be converted.
    if (!arg.length())
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

void cmdFreq(RH_RF95 &rf95, String &arg)
{
    float freq;
    freq = arg.toFloat(); // Convert argument to float. Defaults to 0 if string cannot be converted.
    if (!arg.length())
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

void cmdSend(RH_RF95 &rf95, String &arg)
{
    char conversionBuffer[BUFFER_SIZE];
    uint8_t sendBuffer[BUFFER_SIZE];
    arg.toCharArray(conversionBuffer, BUFFER_SIZE); // Convert string to a char array.
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        // Iterate through the conversionBuffer and cast each char to uint8_t.
        // uint8_t is the datatype expected by the RadioHead library.
        sendBuffer[i] = uint8_t(conversionBuffer[i]);
    }
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify data to be sent.");
        return;
    }
    else
    {
        // Valid frequency has been detected.
        rf95.send(sendBuffer, sizeof(sendBuffer)); // Send the converted buffer
        Serial.println("TX> " + arg);       // Echo the sent data back to serial
        rf95.waitPacketSent();                     // Wait for transmission to finish before returning.
        return;
    }
}