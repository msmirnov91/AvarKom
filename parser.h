#ifndef PARSER_H
#define PARSER_H

#include "QString"
#include "command.h"
#include "mainwindow.h"

class Parser
{
    static int getNormalizedValue(QString value);

public:
    Parser(){}
    static bool parseState(State& state, QString deviceAnswer);
    static int parseInt(QString strInt);
};

#endif // PARSER_H
