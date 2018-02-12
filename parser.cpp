#include "parser.h"
#include "QRegExp"
#include "QStringList"

bool Parser::parseState(QString deviceAnswer){
    QRegExp responseTemplate("\\d{4}:\\d{4}:\\d:\\d\r\n");

    if(!responseTemplate.exactMatch(deviceAnswer)){
        return false;
    }

    QStringList responseParts = deviceAnswer.split(':');

    primSourceLeft = getNormalizedValue(responseParts[0]);
    primSourceRight = getNormalizedValue(responseParts[0]);
    scndSourceLeft = getNormalizedValue(responseParts[1]);
    scndSourceRight = getNormalizedValue(responseParts[1]);
    int stateCode = responseParts[2].toInt();
    source = responseParts[3].toInt();

    if (stateCode == 0){
        state = "auto";
    }
    else if (source == 0){
        state = "prim";
    }
    else if (source == 1){
        state = "scnd";
    }

    /* commented for correct work with new device firmware
    if (stateCode < 0 || stateCode > 1){
        return false;
    }
    */

    if (source < 0 || source > 1){
        return false;
    }

    return true;
}

int Parser::getNormalizedValue(QString value){
    int intValue = value.toInt();
    float normalizedValue = float(intValue)/1024.0;
    return int(normalizedValue*100);
}
