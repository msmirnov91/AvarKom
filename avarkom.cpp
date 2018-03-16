#include "avarkom.h"
#include "QDir"
#include "easylogging++.h"


INITIALIZE_EASYLOGGINGPP


// TODO: fix all log reports!!!!!!!!!!!!!!!!!!!!!!!!!!

Avarkom::Avarkom(QString logPath){
    _sock = new QTcpSocket();
    inBuffer = "";

    processingRequest = false;

    QObject::connect(_sock, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(_sock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(_sock, SIGNAL(readyRead()), this, SLOT(receiveResponse()));
    QObject::connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(onError(QAbstractSocket::SocketError)));

    QString logFileName = logPath + QDir::separator() + "log.log";
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename,
                                       logFileName.toUtf8().constData());

    LOG(INFO)<< "Avarkom device class instantiated";
}

Avarkom::~Avarkom(){
    abortConnection();
    delete _sock;
}

void Avarkom::connect(QHostAddress addr, qint16 port){
    _sock->connectToHost(addr, port);
    LOG(INFO)<< std::string("Connected to ") + addr.toString().toUtf8().constData();
}

void Avarkom::disconnect(){
    _sock->disconnectFromHost();
    LOG(INFO)<< "Disconnected";
}

void Avarkom::abortConnection(){
    _sock->abort();
    LOG(WARNING)<< "Connection aborted";
}

void Avarkom::receiveResponse(){
    QString receivedString = QString(_sock->readAll());

    inBuffer += receivedString;
    if (inBuffer.contains('\n')){
        // we received complete response or the last part of it
        LOG(INFO)<< std::string("Received response: ") + inBuffer.toUtf8().constData();
        currentRequest->setAnswer(inBuffer);
        emit requestProcessingFinished(currentRequest);
        inBuffer = "";
        processingRequest = false;
        processNextCommand();
    }
}

void Avarkom::onError(QAbstractSocket::SocketError error){
    LOG(ERROR)<< "Socket error!";
    emit errorReport("Error!");
}

void Avarkom::sendCommand(QString command){
    QString validCommand = command + "\r";
    _sock->write(validCommand.toUtf8());
    LOG(INFO)<< std::string("Sending command ") + validCommand.toUtf8().constData();
}


void Avarkom::processNewCommand(Command *newCmd){
    requestsQueue.append(newCmd);

    if (processingRequest){
        return;
    }
    else{
        processNextCommand();
    }
}


void Avarkom::processNextCommand(){
    if (requestsQueue.isEmpty() || processingRequest){
        return;
    }

    processingRequest = true;
    currentRequest = requestsQueue.dequeue();
    sendCommand(currentRequest->getCmd());
}
