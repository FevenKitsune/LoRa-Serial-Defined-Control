#include "parser.h"

// Command directives
#define CMD_HELP "help"
#define CMD_TX "tx"
#define CMD_FREQ "freq"
#define CMD_SEND "send"
#define CMD_BW "bw"
#define CMD_SF "sf"

// Data buffers
#define BUFFER_SIZE 64

void ParseCommand(RH_RF95 &rf95, String &serial_data)
{
    String command = PopArgument(serial_data);
    String arg = PopArgument(serial_data);

    /*
    Serial.println("DEBUG:");
    Serial.println("command=" + command);
    Serial.println("arg=" + arg);
    Serial.println("serial_data=" + serial_data);
    */

    // Determine the command root and call the appropriate function.
    // There's probably a cleaner way to do this but I am not a C++ programmer.
    if (command == CMD_HELP)
    {
        CommandHelp();
    }
    else if (command == CMD_TX)
    {
        CommandTX(rf95, arg);
    }
    else if (command == CMD_FREQ)
    {
        CommandFrequency(rf95, arg);
    }
    else if (command == CMD_SEND)
    {
        CommandSend(rf95, arg);
    }
    else if (command == CMD_BW)
    {
        CommandBW(rf95, arg);
    }
    else if (command == CMD_SF)
    {
        CommandSF(rf95, arg);
    }
}

String PopArgument(String &data)
{
    String to_return = data.substring(0, data.indexOf(' '));
    data.remove(0, data.indexOf(' ') + 1 ? data.indexOf(' ') + 1 : data.length());
    // Clears string up to next space. data.indexOf(' ') will return -1 if no space is found.
    // Therefore, data.indexOf(' ') + 1 == 0 or False
    return to_return;
}

void CommandHelp()
{
    Serial.println("LoRa SDCS\tWritten by Will S.");
    Serial.println("help\tRetrieve a list of commands and their functions.");
    Serial.println("tx dBm\tSet transmitter dBm. Valid range: +2 dBm to +20 dBm.");
    Serial.println("freq MHz\tSet transmitter frequency in MHz. Valid range: 137.0 MHz to 1020.0 MHz.");
    Serial.println("send message\tTransmit message through LoRa module.");
    Serial.println("bw Hz\tSet transmitter bandwidth in Hz.");
    Serial.println("sf n\tSet transmitter spreading factor. Valid range: 6 to 12.");
    return;
}

void CommandTX(RH_RF95 &rf95, String &arg)
{
    int tx_level = arg.toInt(); // Convert argument to integer. Defaults to 0 if string cannot be converted.
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify a power output level in dBm. Valid range: +2 dBm to +20 dBm.");
        return;
    }
    else if (tx_level < 2 || tx_level > 20)
    {
        // ERROR: Invalid output level has been given.
        Serial.println("Power output level is invalid! Valid range: +2 dBm to +20 dBm.");
        return;
    }
    else
    {
        // Valid output level has been detected.
        rf95.setTxPower(tx_level, false);
        Serial.println("Power output level: " + String(tx_level) + "dBm");
        return;
    }
}

void CommandFrequency(RH_RF95 &rf95, String &arg)
{
    float freq = arg.toFloat(); // Convert argument to float. Defaults to 0 if string cannot be converted.
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify a frequency in MHz. Valid range: 137.0 MHz to 1020.0 MHz.");
        return;
    }
    else if (freq < 137.0 || freq > 1020.0)
    {
        // ERROR: Invalid frequency has been given.
        Serial.println("Frequency is invalid! Valid range: 137.0 MHz to 1020.0 MHz.");
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

void CommandSend(RH_RF95 &rf95, String &arg)
{
    char conversion_buffer[BUFFER_SIZE];
    uint8_t send_buffer[BUFFER_SIZE];
    arg.toCharArray(conversion_buffer, BUFFER_SIZE); // Convert string to a char array.
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        // Iterate through the conversion_buffer and cast each char to uint8_t.
        // uint8_t is the datatype expected by the RadioHead library.
        send_buffer[i] = uint8_t(conversion_buffer[i]);
    }
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify data to be sent.");
        return;
    }
    else
    {
        // Valid message has been detected.
        rf95.send(send_buffer, sizeof(send_buffer)); // Send the converted buffer
        Serial.println("TX> " + arg);                // Echo the sent data back to serial
        rf95.waitPacketSent();                       // Wait for transmission to finish before returning.
        return;
    }
}

void CommandBW(RH_RF95 &rf95, String &arg)
{
    long bw = arg.toInt();
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify bandwidth in Hz.");
        return;
    }
    else
    {
        // Valid bandwidth has been detected.
        rf95.setSignalBandwidth(bw);
        Serial.println("Bandwidth (Hz): " + String(bw) + "Hz");
        return;
    }
}

void commandSF(RH_RF95 &rf95, String &arg)
{
    uint8_t sf = arg.toInt();
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify a Spreading Factor. Valid range: 6 to 12.");
        return;
    }
    else if (sf < 6 || sf > 12)
    {
        // ERROR: Invalid spreading factor has been given.
        Serial.println("Spreading Factor is invalid! Valid range: 6 to 12.");
        return;
    }
    else
    {
        // Valid spreading factor has been detected.
        rf95.setSpreadingFactor(sf);
        Serial.println("Spreading Factor: " + String(sf));
        return;
    }
}