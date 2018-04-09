#include "logger.h"
#include "QDir"

bool Logger::enabled;

void Logger::init(QString logPath){
    QString logFileName = logPath + QDir::separator() + "log.log";

    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename,
                                       logFileName.toUtf8().constData());
    enabled = true;

}

void Logger::setEnabled(bool isEnabled){
    enabled = isEnabled;
}

bool Logger::isEnabled(){
    return enabled;
}

void Logger::log(QString msg){
    if (isEnabled()){
        LOG(INFO)<< msg.toUtf8().constData();
    }
}
