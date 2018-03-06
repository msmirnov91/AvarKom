#include "avarkom.h"

Avarkom::Avarkom(){
    _sock = new QTcpSocket();
    inBuffer = "";

    processingRequest = false;

    QObject::connect(_sock, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(_sock, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(_sock, SIGNAL(readyRead()), this, SLOT(receiveResponse()));
    QObject::connect(_sock, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(onError(QAbstractSocket::SocketError)));
}

Avarkom::~Avarkom(){
    abortConnection();
    delete _sock;
}

void Avarkom::connect(QHostAddress addr, qint16 port){
    _sock->connectToHost(addr, port);
}

void Avarkom::disconnect(){
    _sock->disconnectFromHost();
}

void Avarkom::abortConnection(){
    _sock->abort();
}

void Avarkom::receiveResponse(){
    QString receivedString = QString(_sock->readAll());

    inBuffer += receivedString;
    if (inBuffer.contains('\n')){
        // we received complete response or the last part of it
        currentRequest->setAnswer(inBuffer);
        emit requestProcessingFinished(currentRequest);
        inBuffer = "";
        processingRequest = false;
        processNextCommand();
    }
}

void Avarkom::onError(QAbstractSocket::SocketError error){
    qDebug() << "error cpde: ";
    qDebug() << error;
    emit errorReport("Error!");
}

void Avarkom::sendCommand(QString command){
    QString validCommand = command + "\r";
    _sock->write(validCommand.toUtf8());
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
