#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#include "parser.h"

// Singleton instance of the radio driver
RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95

// Serial data
String serialData;

void setup()
{
  // Begin the Serial interface.
  Serial.begin(9600);
  Serial.setTimeout(15);
  while (!Serial)
  {
    ; // Wait for serial port to connect.
  }
  Serial.println("Loading...");
  if (!rf95.init()) // Initialize LoRa module and throw error if there's a problem.
  {
    Serial.println("RF95 LoRa Module failed to initialize!");
  }
  Serial.println("Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on");
  Serial.println("Please say a command. Use \"help\" for a list of commands.");
}

void loop()
{
  // Do nothing unless serial is available
  if (Serial.available())
  {
    serialData = Serial.readString(); // Read input string
    Serial.print("$ " + serialData); // Echo input back to user
    parseCommand(rf95, serialData); // Pass string to command parsing program
  }
}
