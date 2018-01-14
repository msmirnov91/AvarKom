#include "request.h"

Request::Request(REQUEST_CODE requestCode){
    code = requestCode;
    cmd = COMMANDS[requestCode];

}

void Request::setParameter(int param){
    if (param > 100){ // may be wrong condition!
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
