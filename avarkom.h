#ifndef DEVICE_H
#define DEVICE_H

#include <QQueue>
#include "QString"
#include "QTcpSocket"
#include "QHostAddress"
#include "command.h"
#include "QTimer"

class Avarkom: public QObject
{
        Q_OBJECT

    QTcpSocket *_sock;
    Command* currentRequest;
    void sendCommand(QString command);
    void sendCommand(QByteArray array);
    QByteArray inBuffer;
    QTimer *answerTimer;

    bool processingRequest;
    QQueue<Command*> requestsQueue;

private slots:
    void onConnected()      {emit connected();}
    void onDisconnected()   {emit disconnected();}

    void receiveResponse();
    void onError(QAbstractSocket::SocketError error);
    void answerTimeout();

public:
    Avarkom();
    ~Avarkom();

    void processNewCommand(Command* newCmd);
    void processNextCommand();

    void connect(QHostAddress addr, qint16 port);
    void disconnect();
    void abortConnection();

signals:
    void connected();
    void disconnected();
    void requestProcessingFinished(Command* req);
    void errorReport(QString errorText);
};

#endif // DEVICE_H
