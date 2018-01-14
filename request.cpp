#include "request.h"

Request::Request(REQUEST_CODE requestCode, bool isQuery){
    code = requestCode;

    cmd = COMMANDS[requestCode];

    this->isQuery = isQuery;

    if (isQuery){
        cmd = QString("?") + cmd;
    }

}

void Request::setParameter(int param){
    // all parameters are in percents
    if (isQuery || param > 100){
        return;
    }

    cmd = QString("%1=%2").arg(cmd).arg(param, 3, 10, QLatin1Char('0'));
}


void Request::setAnswer(QString answer){
    answ = answer;
}


QString Request::getCmd(){
    return cmd;
}


QString Request::getAnswer(){
    return answ;
}


REQUEST_CODE Request::getCode(){
    return code;
}
