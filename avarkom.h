#ifndef DEVICE_H
#define DEVICE_H

#include <QQueue>
#include "QString"
#include "QTcpSocket"
#include "QHostAddress"
#include "request.h"

class Avarkom: public QObject
{
        Q_OBJECT

    QTcpSocket *_sock;
    Request* currentRequest;
    void sendCommand(QString command);
    QString inBuffer;

    bool processingRequest;
    QQueue<Request*> requestsQueue;

private slots:
    void onConnected()      {emit connected();}
    void onDisconnected()   {emit disconnected();}

    void receiveResponse();
    void onError(QAbstractSocket::SocketError error);

public:
    Avarkom();
    ~Avarkom();

    void processRequest(Request* newReq);
    void processNextRequest();

    void connect(QHostAddress addr, qint16 port);
    void disconnect();

signals:
    void connected();
    void disconnected();
    void requestProcessingFinished(Request* req);
    void errorReport(QString errorText);
};

#endif // DEVICE_H
