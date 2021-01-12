#include "parser.h"

// Command directives
#define CMD_HELP "help"
#define CMD_TX "tx"
#define CMD_FREQ "freq"
#define CMD_SEND "send"
#define CMD_BW "bw"
#define CMD_SF "sf"
#define CMD_LOAD "load"
#define CMD_RESET "reset"
#define CMD_CFG "cfg"

// Data buffers
#define BUFFER_SIZE 64

// Flash Storage objects
bool has_loaded = false;
FlashStorage(tx_storage, int);
FlashStorage(freq_storage, float);
FlashStorage(bw_storage, long);
FlashStorage(sf_storage, uint8_t);

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
    else if (command == CMD_LOAD)
    {
        CommandLoad(rf95);
    }
    else if (command == CMD_RESET)
    {
        CommandReset(rf95);
    }
    else if (command == CMD_CFG)
    {
        CommandCfg(rf95);
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
    Serial.println("LoRa Serial Defined Control Software (SDCS)\tWritten by Will S.");
    Serial.println("===============================================================");
    Serial.println("help            Retrieve a list of commands and their functions.");
    Serial.println("tx [dBm]        Set transmitter dBm. Valid range: +2 dBm to +20 dBm.");
    Serial.println("freq [MHz]      Set transmitter frequency in MHz. Valid range: 137.0 MHz to 1020.0 MHz.");
    Serial.println("send [message]  Transmit message through LoRa module.");
    Serial.println("get [timeout]   Recieve a message. Timeout given in ms. Valid range: 1 ms to 30000 ms.");
    Serial.println("bw [Hz]         Set transmitter bandwidth in Hz.");
    Serial.println("sf [n]          Set transmitter spreading factor. Valid range: 6 to 12.");
    Serial.println("load            Retrieves the last assigned values for each command and applies them.");
    Serial.println("reset           Wipes load retrieval storage. Values are assigned when their respective command is called.");
    Serial.println("cfg             Prints load retrieval storage. Must have loaded since last power cycle.");
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
        tx_storage.write(tx_level);
        Serial.println("Power output level: " + String(tx_level) + " dBm");
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
        freq_storage.write(freq);
        Serial.println("Frequency (MHz): " + String(freq) + " MHz");
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

void CommandGet(RH_RF95 &rf95, String &arg)
{
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    long timeout = arg.toInt();
    if (!arg.length())
    {
        // ERROR: No arguments have been given.
        Serial.println("Please specify timeout in ms. Valid range: 1 ms to 30000 ms.");
        return;
    }
    else if (timeout < 1 || timeout > 30000)
    {
        // ERROR: Invalid timeout has been given.
        Serial.println("Timeout is invalid! Valid range: 1 ms to 30000 ms.");
        return;
    }
    else
    {
        if (rf95.waitAvailableTimeout(timeout))
        {
            if (rf95.recv(buf, &len))
            {
                Serial.print("RX> ");
                Serial.println((char*)buf);
                Serial.print("RSSI = ");
                Serial.println(rf95.lastRssi(), DEC);
            }
            else
            {
                Serial.println("RX failed! Unable to recieve message.");
            }
        }
        else
        {
            Serial.println("Timeout has been reached. No message recieved.");
        }
        
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
        bw_storage.write(bw);
        Serial.println("Bandwidth (Hz): " + String(bw) + " Hz");
        return;
    }
}

void CommandSF(RH_RF95 &rf95, String &arg)
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
        sf_storage.write(sf);
        Serial.println("Spreading Factor: " + String(sf));
        return;
    }
}

void CommandLoad(RH_RF95 &rf95)
{
    int recovered_tx = tx_storage.read();
    float recovered_freq = freq_storage.read();
    long recovered_bw = bw_storage.read();
    uint8_t recovered_sf = sf_storage.read();
    has_loaded = true;

    // Check recovered power output level and update.
    if (recovered_tx == 0)
    {
        Serial.println("Power output level not saved, ignoring.");
    }
    else
    {
        rf95.setTxPower(recovered_tx);
        Serial.println("Recovered power output level: " + String(recovered_tx) + " dBm");
    }

    // Check recovered frequency value and update.
    if (recovered_freq == 0.0)
    {
        Serial.println("Frequency not saved, ignoring.");
    }
    else
    {
        rf95.setFrequency(recovered_freq);
        Serial.println("Recovered frequency: " + String(recovered_freq) + " MHz");
    }

    // Check recovered bandwidth value and update
    if (recovered_bw == 0)
    {
        Serial.println("Bandwidth not saved, ignoring.");
    }
    else
    {
        rf95.setTxPower(recovered_bw);
        Serial.println("Recovered bandwidth: " + String(recovered_bw) + " Hz");
    }

    // Check recovered spreading factor value and update
    if (recovered_sf == 0)
    {
        Serial.println("Spreading factor not saved, ignoring.");
    }
    else
    {
        rf95.setTxPower(recovered_sf);
        Serial.println("Recovered spreading factor: " + String(recovered_sf) + " dBm");
    }
}

void CommandReset(RH_RF95 &rf95)
{
    tx_storage.write(0);
    freq_storage.write(0.0);
    bw_storage.write(0);
    sf_storage.write(0);
    Serial.println("System configuration reset! Default values will be loaded on next startup.");
}

void CommandCfg(RH_RF95 &rf95)
{
    if (!has_loaded)
    {
        Serial.println("Configuration hasn't been loaded since last power cycle. Current settings cannot be determined.");
        Serial.println("Run the \"load\" command to ensure current settings correspond to known configuration.");
    }
    else
    {
        int recovered_tx = tx_storage.read();
        float recovered_freq = freq_storage.read();
        long recovered_bw = bw_storage.read();
        uint8_t recovered_sf = sf_storage.read();

        // Check recovered power output level and print.
        if (recovered_tx == 0)
        {
            Serial.println("Power output level not saved, assume default value of 13 dBm.");
        }
        else
        {
            Serial.println("Current power output level: " + String(recovered_tx) + " dBm");
        }

        // Check recovered frequency value and print.
        if (recovered_freq == 0.0)
        {
            Serial.println("Frequency not saved, assume default value of 434.0 Mhz.");
        }
        else
        {
            Serial.println("Recovered frequency: " + String(recovered_freq) + " MHz");
        }

        // Check recovered bandwidth value and print
        if (recovered_bw == 0)
        {
            Serial.println("Bandwidth not saved, assume default value of 125000 Hz.");
        }
        else
        {
            Serial.println("Recovered bandwidth: " + String(recovered_bw) + " Hz");
        }

        // Check recovered spreading factor value and print
        if (recovered_sf == 0)
        {
            Serial.println("Spreading factor not saved, assume default value of 7.");
        }
        else
        {
            Serial.println("Recovered spreading factor: " + String(recovered_sf));
        }
    }
}