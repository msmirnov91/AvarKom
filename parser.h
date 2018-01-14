#ifndef PARSER_H
#define PARSER_H

#include "QString"
#include "request.h"

class Parser
{
    int primSourceLeft;
    int primSourceRight;
    int scndSourceLeft;
    int scndSourceRight;
    int source;
    QString state;

    int getNormalizedValue(QString value);

public:
    Parser(){}
    bool parseState(QString deviceRequest);

    QString getState(){return state;}
    int getCurrentSource() {return source;}
    int get1sourceLeftLevel() {return primSourceLeft;}
    int get1sourceRightLevel(){return primSourceRight;}
    int get2sourceLeftLevel() {return scndSourceLeft;}
    int get2sourceRightLevel(){return scndSourceRight;}
};

#endif // PARSER_H
