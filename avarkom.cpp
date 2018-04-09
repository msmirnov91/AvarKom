#include "avarkom.h"
#include "logger.h"


Avarkom::Avarkom(){
    _sock = new QTcpSocket();
    inBuffer = "";

    processingRequest = false;
    answerTimer = new QTimer();
    answerTimer->setInterval(1500);

    QObject::connect(_sock, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(_sock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(_sock, SIGNAL(readyRead()), this, SLOT(receiveResponse()));
    QObject::connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(onError(QAbstractSocket::SocketError)));
    QObject::connect(answerTimer, SIGNAL(timeout()), this, SLOT(answerTimeout()));

    Logger::log("Avarkom device class instantiated");
}

Avarkom::~Avarkom(){
    abortConnection();
    delete _sock;
    delete answerTimer;
}

void Avarkom::connect(QHostAddress addr, qint16 port){
    _sock->connectToHost(addr, port);
    Logger::log("Connected to " + addr.toString());
}

void Avarkom::disconnect(){
    _sock->disconnectFromHost();
    Logger::log("Disconnected");
}

void Avarkom::abortConnection(){
    _sock->abort();
    Logger::log("Connection aborted");
}

void Avarkom::receiveResponse(){
    inBuffer.append(_sock->readAll());
    if (inBuffer.length() >= Command::replyLength){
        // we received complete response or the last part of it
        answerTimer->stop();
        Logger::log("Received response");
        currentRequest->setAnswer(inBuffer);
        currentRequest->setAnswered(true);
        emit requestProcessingFinished(currentRequest);
        inBuffer.clear();
        processingRequest = false;
        processNextCommand();
    }
}

void Avarkom::onError(QAbstractSocket::SocketError error){
    Logger::log("Socket error");
    emit errorReport("Error!");
}


void Avarkom::sendCommand(QByteArray array){
    _sock->write(array, Command::commandLength);
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
    sendCommand(currentRequest->getByteArray());
    answerTimer->start();
}

void Avarkom::answerTimeout(){
    answerTimer->stop();
    currentRequest->setAnswered(false);
    emit requestProcessingFinished(currentRequest);
}
