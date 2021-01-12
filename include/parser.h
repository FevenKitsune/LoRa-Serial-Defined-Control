#ifndef PARSER_H
#define PARSER_H
#include <RH_RF95.h>
#include <Arduino.h>
void ParseCommand(RH_RF95 &rf95, String &serial_data);
String PopArgument(String &data);
void CommandHelp();
void CommandTX(RH_RF95 &rf95, String &arg);
void CommandFrequency(RH_RF95 &rf95, String &arg);
void CommandSend(RH_RF95 &rf95, String &arg);
void CommandBW(RH_RF95 &rf95, String &arg);

#endif