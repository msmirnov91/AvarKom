#ifndef LOGGER_H
#define LOGGER_H


#include <QString>
#include "easylogging++.h"

class Logger
{
    static bool enabled;
public:
    static void init(QString logPath);
    static void setEnabled(bool isEnabled);
    static bool isEnabled();
    static void log(QString msg);
};

#endif // LOGGER_H
