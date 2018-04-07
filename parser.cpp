#include "parser.h"

void Parser::parseState(State &state, QByteArray deviceAnswer){

    int index = 0;
    state.fstSourceLeft = getSoundLevel(deviceAnswer, index);
    state.fstSourceRight = getSoundLevel(deviceAnswer, index);
    state.sndSourceLeft = getSoundLevel(deviceAnswer, index);
    state.sndSourceRight = getSoundLevel(deviceAnswer, index);

    state.currState = deviceAnswer[8];
    state.currSource = deviceAnswer[9];
    state.relayState = deviceAnswer[10];
}

void Parser::parseSetpoints(Setpoints& state, QByteArray deviceAnswer){
    // thresholds are in percents
    state.loudThreshold = deviceAnswer[0];
    state.quietThreshold = deviceAnswer[1];
    // timeouts are in seconds
    state.loudTimeout = deviceAnswer[2];
    state.quietTimeout = deviceAnswer[3];
}

void Parser::parseNetworkSettings(NetworkSettings& settings, QByteArray deviceAnswer){
    settings.newAddressString = makeOctetsString(deviceAnswer, 0);
    settings.newPort = deviceAnswer[4];
    settings.netmask = makeOctetsString(deviceAnswer, 5);
    settings.gateway = makeOctetsString(deviceAnswer, 9);
    settings.useDhcp = deviceAnswer[13];
}

QString Parser::makeOctetsString(QByteArray answ, int beginning){
    QString result = "%1.%2.%3.%4";
    for (int i = beginning; i < beginning + 4; i++){
        result = result.arg((quint8)answ[i]);
    }
    return result;
}

unsigned int Parser::getSoundLevel(QByteArray deviceAnswer, int& index){
    unsigned int result = 0;
    result |= (quint8)deviceAnswer[index++];

    unsigned int shiftedByte = ((quint8)deviceAnswer[index++]) << 8;
    result |= shiftedByte;

    // make percent from absolute value
    result = (float)result/10.23; // TODO: fix this as soon as possible!!

    return result;
}
