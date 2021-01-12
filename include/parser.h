#ifndef PARSER_H
#define PARSER_H
#include <RH_RF95.h>
#include <Arduino.h>
#include <FlashStorage.h>
void ParseCommand(RH_RF95 &rf95, String &serial_data);
String PopArgument(String &data);
void CommandHelp();
void CommandTX(RH_RF95 &rf95, String &arg);
void CommandFrequency(RH_RF95 &rf95, String &arg);
void CommandSend(RH_RF95 &rf95, String &arg);
void CommandGet(RH_RF95 &rf95, String &arg);
void CommandBW(RH_RF95 &rf95, String &arg);
void CommandSF(RH_RF95 &rf95, String &arg);
void CommandLoad(RH_RF95 &rf95);
void CommandReset(RH_RF95 &rf95);
void CommandCfg(RH_RF95 &rf95);

#endif