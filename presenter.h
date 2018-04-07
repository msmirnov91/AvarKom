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
    MainWindow *view;

    void connectViewSignals();
    void connectAvarkomSignals();
    void requestAllParams();

public:
    Presenter(Avarkom *device, MainWindow *UIView);
    ~Presenter();

public slots:
    void makeConnecton();
    void makeDisconnecton();
    void connectionTimeout();
    void setPollingTime(int pollingTime);

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
