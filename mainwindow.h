#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSettings"

struct NetworkSettings{
    int useDhcp;
    QString newAddressString;
    int newPort;
    QString netmask;
    QString gateway;
};

struct Setpoints{
    int loudThreshold;
    int quietThreshold;
    int loudTimeout;
    int quietTimeout;
};

struct State{
    enum {PRIM, SCND, AUTO};

    int currSource;
    int fstSourceLeft;
    int fstSourceRight;
    int sndSourceLeft;
    int sndSourceRight;
    int relayState;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSettings storedConnectionParams;

    Ui::MainWindow *ui;
    QString activeStyleSheet;
    QString passiveStyleSheet;

private slots:
    void emitChangeStateSignal();
    void emitChangeNetworkSettings();
    void emitChangeSetpoints();

    void enableIpString(bool useDhcp);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getAddressString();
    qint16 getPort();

    void changeToConnetedMode();
    void changeToDisconnectedMode();
    void changeToConnectionMode();

    void setState(State state);
    void setSetpoints(Setpoints setpoints);
    void setNetworkSettings(NetworkSettings storedConnectionParams);
    void setStatusText(QString statusText);

signals:
    void connectionRequest();
    void disconnectionRequest();

    void changeStateRequest(QString newState);
    void toggleRelay(bool);
    void updateSetpoints(Setpoints Setpoints);
    void updateNetworkSettings(NetworkSettings params);
    void stateRequest();
};

#endif // MAINWINDOW_H
