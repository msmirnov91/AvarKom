#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <map>

enum REQUEST_CODE{SET_AUTO, SET_PRIM, SET_SCND, GET_STATE,
            SET_SOUND_THR, SET_LOUD_THR, GET_SOUND_THR, GET_LOUD_THR};


class Request
{
    REQUEST_CODE code;
    QString cmd;
    QString answ;

    bool isQuery;

    std::map<REQUEST_CODE, QString> COMMANDS = {
        {SET_AUTO, "AUTO"},
        {SET_PRIM, "PRIM"},
        {SET_SCND, "SCND"},
        {GET_STATE, "STAT"},
        {SET_SOUND_THR, "STAT"},
        {SET_LOUD_THR, "STAT"},
        {GET_SOUND_THR, "STAT"},
        {GET_LOUD_THR, "STAT"}
    };

public:
    Request(REQUEST_CODE requestCode, bool isQuery=false);
    void setParameter(int param);
    void setAnswer(QString answer);
    QString getCmd();
    QString getAnswer();
    REQUEST_CODE getCode();
};

#endif // REQUEST_H
