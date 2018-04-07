#ifndef PARSER_H
#define PARSER_H

#include "QString"
#include "command.h"
#include "mainwindow.h"

class Parser
{
    static QString makeOctetsString(QByteArray answ, int beginning);
    static unsigned int getSoundLevel(QByteArray deviceAnswer, int& index);

public:
    Parser(){}
    static void parseState(State& state, QByteArray deviceAnswer);
    static void parseSetpoints(Setpoints& state, QByteArray deviceAnswer);
    static void parseNetworkSettings(NetworkSettings& settings, QByteArray deviceAnswer);
};

#endif // PARSER_H
