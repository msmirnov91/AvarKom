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
    QTimer *watchDog;

    Avarkom *avarkomPMS;
    Parser *responseParser;
    MainWindow *view;

    State currentAvarkomState;
    Setpoints currentSetpoints;
    NetworkSettings currentNetworkSettings;

    void connectViewSignals();
    void connectAvarkomSignals();

    void updateViewState(QString deviceAnswer);
    void requestAllParams();

public:
    Presenter(Avarkom *device, Parser *parser, MainWindow *UIView, int updateTime);
    ~Presenter();

public slots:
    void makeConnecton();
    void makeDisconnecton();
    void connectionTimeout();

    void requestState();
    void handleExecutedRequest(Command* executedRequest);
    void reportError(QString error);
    void changeDeviceState(QString newState);
    void toggleRelay(bool state);
    void updateDeviceSetpoints(Setpoints setpoints);
    void updateDeviceNetworkSettings(NetworkSettings settings);

    void onConnect();
    void onDisconnect();
};

#endif // PRESENTER_H
