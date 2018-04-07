#include "command.h"
#include "QStringList"

int Command::commandLength = 9;
int Command::replyLength = 14;

Command::Command(COMMAND_CODE commandCode){
    code = commandCode;
}

Command::Command(COMMAND_CODE commandCode, int param){
    // parameters are set in percents
    param = param <= 100 ? param : 100;

    code = commandCode;
    parameter = (qint8) param;
}

Command::Command(COMMAND_CODE commandCode, QString param){
    code = commandCode;
    QStringList parameterParts = param.split(".");
    for (auto i = 0; i < 4; i++){
        qint8 octet = parameterParts[i].toUInt();
        octets.append(octet);
    }
}


void Command::setAnswer(QByteArray answer){
    answ = answer;
}


QByteArray Command::getByteArray(){
    const qint8 startBytes[] = {0x06, 0x16, 0x06}; //ACK, SYN, ACK
    QByteArray resultArray(reinterpret_cast<const char*>(startBytes), sizeof(startBytes));
    resultArray.append((char)code);

    switch (code) {
    case LOUD_THR:
    case QUIET_THR:
    case LOUD_TIMEOUT:
    case QUIET_TIMEOUT:
    case DHCP:
    case PORT:
    case RELAY:
        resultArray.append((char)parameter);
        break;
    case IP_ADDR:
    case NETMASK:
    case GATEWAY:
        resultArray.append(octets);
        break;
    default:
        break;
    }

    while(resultArray.length() < Command::commandLength - 1){
        resultArray.append('\0');
    }

    resultArray.append(0x0d); // CR
    return resultArray;
}


QByteArray Command::getAnswer(){
    return answ;
}


COMMAND_CODE Command::getCode(){
    return code;
}

void Command::setAnswered(bool isAnswered){
    answered = isAnswered;
}

bool Command::isAnswered(){
    return answered;
}
