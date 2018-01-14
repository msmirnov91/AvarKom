#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <map>

enum REQUEST_CODE{SET_AUTO, SET_PRIM, SET_SCND, STATE,
                  LOUD_THR, QUIET_THR, LOUD_TIMEOUT, QUIET_TIMEOUT,
                  SET_IP, SET_PORT};


class Request
{
    REQUEST_CODE code;
    QString cmd;
    QString answ;

    std::map<REQUEST_CODE, QString> COMMANDS = {
        {SET_AUTO, "AUTO"},
        {SET_PRIM, "PRIM"},
        {SET_SCND, "SCND"},
        {STATE, "STAT"},
        {LOUD_THR, "LTHR"},
        {QUIET_THR, "QTHR"},
        {LOUD_TIMEOUT, "LTIM"},
        {QUIET_TIMEOUT, "QTIM"},
        {SET_IP, "ADDR"},
        {SET_PORT, "PORT"}
    };

public:
    Request(REQUEST_CODE requestCode);
    void setParameter(int param);
    void setAnswer(QString answer);
    QString getCmd();
    QString getAnswer();
    REQUEST_CODE getCode();
};

#endif // REQUEST_H
