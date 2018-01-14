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
    void emitConnectionSignal()    {emit connectionRequest();}
    void emitDisconnectionSignal() {emit disconnectionRequest();}

    void emitChangeStateSignal();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeToConnetedMode();
    void changeToDisconnectedMode();

    void setState(QString currentState);
    void set1SourceLeftLevel(int level);
    void set1SourceRightLevel(int level);
    void set2SourceLeftLevel(int level);
    void set2SourceRightLevel(int level);
    void setErrorText(QString errorText);

    QString getAddressString();
    qint16 getPort();


signals:
    void connectionRequest();
    void disconnectionRequest();

    void changeStateRequest(QString newState);
    void stateRequest();
};

#endif // MAINWINDOW_H
