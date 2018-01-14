#ifndef PRESENTER_H
#define PRESENTER_H

#include "QTimer"
#include "avarkom.h"
#include "parser.h"
#include "mainwindow.h"
#include "request.h"

class Presenter: public QObject
{
    Q_OBJECT

    QTimer *pollingTimer;
    int pollingTime;
    Avarkom *avarkomPMS;
    Parser *responseParser;
    MainWindow *view;

    void connectViewSignals();
    void connectAvarkomSignals();

    void updateState(QString deviceAnswer);

public:
    Presenter(Avarkom *device, Parser *parser, MainWindow *UIView, int updateTime);
    ~Presenter();

public slots:
    void makeConnecton();
    void makeDisconnecton();

    void requestState();
    void handleExecutedRequest(Request* executedRequest);
    void reportError(QString error);
    void changeDeviceState(QString newState);

    void indicateConnection();
    void indicateDisconnection();
};

#endif // PRESENTER_H
