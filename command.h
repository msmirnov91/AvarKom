#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <map>

enum COMMAND_CODE{SET_PRIM,
                  SET_SCND,
                  SET_AUTO,
                  STATE,
                  NETWORK_STATE,
                  COMMUTATION_STATE,
                  LOUD_THR,
                  QUIET_THR,
                  LOUD_TIMEOUT,
                  QUIET_TIMEOUT,
                  DHCP,
                  IP_ADDR,
                  PORT,
                  NETMASK,
                  GATEWAY,
                  RELAY};


class Command
{
    COMMAND_CODE code;
    QByteArray answ;
    QByteArray octets;
    qint8 parameter;
    bool answered;

public:
    static int commandLength;
    static int replyLength;

    Command(COMMAND_CODE commandCode);
    Command(COMMAND_CODE commandCode, int param);
    Command(COMMAND_CODE commandCode, QString param);
    void setAnswer(QByteArray answer);
    void setAnswered(bool isAnswered);
    bool isAnswered();
    QByteArray getByteArray();
    QByteArray getAnswer();
    COMMAND_CODE getCode();
};

#endif // COMMAND_H
