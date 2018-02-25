#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QString activeStyleSheet;
    QString passiveStyleSheet;

private slots:
    /*void emitConnectionSignal()      {emit connectionRequest();}
    void emitDisconnectionSignal()   {emit disconnectionRequest();}
    void emitUpdateSetpointsSignal() {emit updateSetpoints();}
    void emitUpdateNetworkSignal()   {emit updateNetworkSettings();}*/

    void emitChangeStateSignal();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getAddressString();
    qint16 getPort();

    void changeToConnetedMode();
    void changeToDisconnectedMode();

    void setState(QString currentState);
    void set1SourceLeftLevel(int level);
    void set1SourceRightLevel(int level);
    void set2SourceLeftLevel(int level);
    void set2SourceRightLevel(int level);
    void setErrorText(QString errorText);

    int getLoudThreshold();
    int getQuietThreshold();
    int getLoudTimeout();
    int getQuietTimeout();

    QString getNewAddressString();
    int getNewPort();
    QString getNetmask();
    QString getGateway();


signals:
    void connectionRequest();
    void disconnectionRequest();

    void changeStateRequest(QString newState);
    void updateSetpoints();
    void updateNetworkSettings();
    void stateRequest();
};

#endif // MAINWINDOW_H
