#ifndef PARSER_H
#define PARSER_H
#include <RH_RF95.h>
#include <Arduino.h>
void parseCommand(RH_RF95 &rf95, String &serialData);
String popArg(String &data);
void cmdHelp();
void cmdTx(RH_RF95 &rf95, String &arg);
void cmdFreq(RH_RF95 &rf95, String &arg);
void cmdSend(RH_RF95 &rf95, String &arg);

#endif