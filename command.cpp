#include "command.h"

Command::Command(COMMAND_CODE commandCode){
    code = commandCode;
    cmd = COMMANDS[commandCode];

}

Command::Command(COMMAND_CODE commandCode, int param){
    param = param <= 100 ? param : 100;

    code = commandCode;
    cmd = QString("%1=%2").arg(COMMANDS[commandCode])
                          .arg(param, 3, 10, QLatin1Char('0'));
}

Command::Command(COMMAND_CODE commandCode, QString param){
    code = commandCode;
    cmd = COMMANDS[commandCode] + "=" + param;
}


void Command::setAnswer(QString answer){
    answ = answer;
}


QString Command::getCmd(){
    return cmd;
}


QString Command::getAnswer(){
    return answ;
}


COMMAND_CODE Command::getCode(){
    return code;
}
