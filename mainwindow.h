#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSettings"

struct NetworkSettings{
    unsigned int useDhcp;
    QString newAddressString;
    unsigned int newPort;
    QString netmask;
    QString gateway;
};

struct Setpoints{
    unsigned int loudThreshold;
    unsigned int quietThreshold;
    unsigned int loudTimeout;
    unsigned int quietTimeout;
};

struct State{
    enum {PRIM, SCND, AUTO};

    unsigned int fstSourceLeft;
    unsigned int fstSourceRight;
    unsigned int sndSourceLeft;
    unsigned int sndSourceRight;
    unsigned int currState;
    unsigned int currSource;
    unsigned int relayState;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSettings* storedAppSettings;

    Ui::MainWindow *ui;
    QString activeStyleSheet;
    QString passiveStyleSheet;

private slots:
    void emitChangeStateSignal();
    void emitChangeNetworkSettings();
    void emitChangeSetpoints();

    void enableIpString(bool useDhcp);
    void emitPollingTimeChanged(int time);
    void enableLogging(bool enable);

public:
    explicit MainWindow(QString settingsPath, QWidget *parent = 0);
    ~MainWindow();

    QString getAddressString();
    qint16 getPort();

    void changeToConnetedMode();
    void changeToDisconnectedMode();
    void changeToConnectionMode();

    void setState(State state);
    void setSetpoints(Setpoints setpoints);
    void setNetworkSettings(NetworkSettings storedAppSettings);
    void setStatusText(QString statusText);

    int getPollingTime();

signals:
    void connectionRequest();
    void disconnectionRequest();
    void pollingTimeChanged(int);

    void changeStateRequest(QString newState);
    void toggleRelay(bool);
    void updateSetpoints(Setpoints Setpoints);
    void updateNetworkSettings(NetworkSettings params);
    void stateRequest();
};

#endif // MAINWINDOW_H
