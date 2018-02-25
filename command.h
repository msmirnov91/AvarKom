#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <map>

enum COMMAND_CODE{SET_PRIM,
                  SET_SCND,
                  SET_AUTO,
                  STATE,
                  LOUD_THR,
                  QUIET_THR,
                  LOUD_TIMEOUT,
                  QUIET_TIMEOUT,
                  IP_ADDR,
                  PORT,
                  NETMASK,
                  GATEWAY,
                  RELAY};


class Command
{
    COMMAND_CODE code;
    QString cmd;
    QString answ;

    std::map<COMMAND_CODE, QString> COMMANDS = {
        {SET_PRIM, "PRIM"},
        {SET_SCND, "SCND"},
        {SET_AUTO, "AUTO"},
        {STATE, "STAT"},
        {LOUD_THR, "LTHR"},
        {QUIET_THR, "QTHR"},
        {LOUD_TIMEOUT, "LTIM"},
        {QUIET_TIMEOUT, "QTIM"},
        {IP_ADDR, "ADDR"},
        {PORT, "PORT"},
        {NETMASK, "MASK"},
        {GATEWAY, "GATE"},
        {RELAY, "RLAY"}
    };

public:
    Command(COMMAND_CODE commandCode);
    Command(COMMAND_CODE commandCode, int param);
    Command(COMMAND_CODE commandCode, QString param);
    void setAnswer(QString answer);
    QString getCmd();
    QString getAnswer();
    COMMAND_CODE getCode();
};

#endif // COMMAND_H
