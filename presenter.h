#ifndef PRESENTER_H
#define PRESENTER_H

#include "QTimer"
#include "avarkom.h"
#include "parser.h"
#include "mainwindow.h"
#include "command.h"

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

    void updateViewState(QString deviceAnswer);

public:
    Presenter(Avarkom *device, Parser *parser, MainWindow *UIView, int updateTime);
    ~Presenter();

public slots:
    void makeConnecton();
    void makeDisconnecton();

    void requestState();
    void handleExecutedRequest(Command* executedRequest);
    void reportError(QString error);
    void changeDeviceState(QString newState);
    void updateDeviceSetpoints();
    void updateDeviceNetworkSettings();

    void indicateConnection();
    void indicateDisconnection();
};

#endif // PRESENTER_H
