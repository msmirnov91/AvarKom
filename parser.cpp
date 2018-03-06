#include "parser.h"
#include "QRegExp"
#include "QStringList"

bool Parser::parseState(State &state, QString deviceAnswer){

    QRegExp responseTemplate("\\d{4}:\\d{4}:\\d{4}:\\d{4}:\\d:\\d\r\n");

    if(!responseTemplate.exactMatch(deviceAnswer)){
        return false;
    }

    QStringList responseParts = deviceAnswer.split(':');

    state.fstSourceLeft = getNormalizedValue(responseParts[0]);
    state.fstSourceRight = getNormalizedValue(responseParts[1]);
    state.sndSourceLeft = getNormalizedValue(responseParts[2]);
    state.sndSourceRight = getNormalizedValue(responseParts[3]);
    state.currSource = responseParts[4].toInt();
    state.relayState = responseParts[5].toInt();

    return true;
}

int Parser::parseInt(QString strInt){
    return strInt.toInt();
}

int Parser::getNormalizedValue(QString value){
    int intValue = value.toInt();
    float normalizedValue = float(intValue)/1024.0;
    return int(normalizedValue*100);
}
