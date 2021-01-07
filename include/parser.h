#ifndef PARSER_H
#define PARSER_H
#include <RH_RF95.h>
#include <Arduino.h>
void parseCommand(RH_RF95 &rf95, String serialData);
void cmdHelp();
void cmdTx(RH_RF95 &rf95, String serialData);
void cmdFreq(RH_RF95 &rf95, String serialData);
void cmdSend(RH_RF95 &rf95, String serialData);

#endif