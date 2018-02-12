#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    // connect/disconnect signals
    QObject::connect(ui->connectButton, SIGNAL(clicked()),
                     this, SLOT(emitConnectionSignal()));
    QObject::connect(ui->disconnectButton, SIGNAL(clicked()),
                     this, SLOT(emitDisconnectionSignal()));

    // control signals
    QObject::connect(ui->autoButton, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->source1Button, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->source2Button, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->setCommutParamsButton, SIGNAL(clicked()),
                     this, SLOT(emitUpdateSetpointsSignal()));
    QObject::connect(ui->setNetworkParamsButton, SIGNAL(clicked()),
                     this, SLOT(emitUpdateNetworkSignal()));

    activeStyleSheet = "background-color: #3cbaa2;";
    passiveStyleSheet = "";

    changeToDisconnectedMode();
}

MainWindow::~MainWindow(){
    delete ui;
}

QString MainWindow::getAddressString(){
    return ui->ipLineEdit->text();
}

qint16 MainWindow::getPort(){
    return qint16(ui->portSpinBox->value());
}

void MainWindow::emitChangeStateSignal(){
    if (QObject::sender() == ui->autoButton){
        emit changeStateRequest("auto");
    }
    else if (QObject::sender() == ui->source1Button){
        emit changeStateRequest("prim");
    }
    else if (QObject::sender() == ui->source2Button){
        emit changeStateRequest("scnd");
    }
}

void MainWindow::changeToConnetedMode(){
    ui->stateSelectGB->setEnabled(true);
    ui->indication1GB->setEnabled(true);
    ui->indication2GB->setEnabled(true);

    ui->connParamGB->setEnabled(false);
    ui->connStateLabel->setText("Соединен");

}

void MainWindow::changeToDisconnectedMode(){
    ui->autoButton->setStyleSheet(passiveStyleSheet);
    ui->source1Button->setStyleSheet(passiveStyleSheet);
    ui->source2Button->setStyleSheet(passiveStyleSheet);

    ui->stateSelectGB->setEnabled(false);
    ui->indication1GB->setEnabled(false);
    ui->indication2GB->setEnabled(false);

    ui->connParamGB->setEnabled(true);
    ui->connStateLabel->setText("Разъединен");
}

void MainWindow::setState(QString currentState){
    ui->autoButton->setStyleSheet(passiveStyleSheet);
    ui->source1Button->setStyleSheet(passiveStyleSheet);
    ui->source2Button->setStyleSheet(passiveStyleSheet);

    if (currentState == "auto"){
        ui->autoButton->setStyleSheet(activeStyleSheet);
    }
    else if(currentState == "prim"){
        ui->source1Button->setStyleSheet(activeStyleSheet);
    }
    else if(currentState == "scnd"){
        ui->source2Button->setStyleSheet(activeStyleSheet);
    }
}

void MainWindow::set1SourceLeftLevel(int level){
    ui->source1LevelLeft->setValue(level);
}

void MainWindow::set1SourceRightLevel(int level){
    ui->source1LevelRight->setValue(level);
}

void MainWindow::set2SourceLeftLevel(int level){
    ui->source2LevelLeft->setValue(level);
}

void MainWindow::set2SourceRightLevel(int level){
    ui->source2LevelRight->setValue(level);
}

void MainWindow::setErrorText(QString errorText){
    ui->connStateLabel->setText(errorText);
}

int MainWindow::getLoudThreshold(){
    return ui->loudThresholdSpinBox->value();
}

QString MainWindow::getNewAddressString(){
    return ui->newIpLineEdit->text();
}

int MainWindow::getNewPort(){
    return ui->newPortSpinBox->value();
}
