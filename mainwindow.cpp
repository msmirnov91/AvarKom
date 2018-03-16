#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QValidator>
#include "QDir"


MainWindow::MainWindow(QString settingsPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    storedConnectionParams = new QSettings(settingsPath + QDir::separator() +
                                           "settings.cfg", QSettings::IniFormat);

    // connect/disconnect signals
    QObject::connect(ui->connectButton, SIGNAL(clicked()),
                     this, SIGNAL(connectionRequest()));
    QObject::connect(ui->disconnectButton, SIGNAL(clicked()),
                     this, SIGNAL(disconnectionRequest()));
    QObject::connect(ui->dhcpCB, SIGNAL(toggled(bool)),
                     this, SLOT(enableIpString(bool)));
    QObject::connect(ui->pollingTimeSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(emitPollingTimeChanged(int)));

    // control signals
    QObject::connect(ui->autoButton, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->source1Button, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->source2Button, SIGNAL(clicked()),
                     this, SLOT(emitChangeStateSignal()));
    QObject::connect(ui->relayRB, SIGNAL(toggled(bool)),
                     this, SIGNAL(toggleRelay(bool)));
    QObject::connect(ui->setCommutParamsButton, SIGNAL(clicked()),
                     this, SLOT(emitChangeSetpoints()));
    QObject::connect(ui->setNetworkParamsButton, SIGNAL(clicked()),
                     this, SLOT(emitChangeNetworkSettings()));

    activeStyleSheet = "background-color: #3cbaa2;";
    passiveStyleSheet = "";

    QString addr = storedConnectionParams->value("IPADDR", "10.0.0.100").toString();
    int port = storedConnectionParams->value("PORT", 90).toInt();
    ui->ipLineEdit->setText(addr);
    ui->portSpinBox->setValue(port);

    int pollingTime = storedConnectionParams->value("POLLINGTIME", 5000).toInt();
    ui->pollingTimeSlider->setValue(pollingTime);

    changeToDisconnectedMode();
}

void MainWindow::_setValidators(){
    return;
}

bool MainWindow::_addressesAreValid(){
    return true;
}

MainWindow::~MainWindow(){
    delete ui;
    delete storedConnectionParams;
}

QString MainWindow::getAddressString(){
    return ui->ipLineEdit->text();
}

qint16 MainWindow::getPort(){
    return qint16(ui->portSpinBox->value());
}

void MainWindow::emitChangeStateSignal(){
    // TODO: fix this!!!
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

void MainWindow::emitChangeNetworkSettings(){
    NetworkSettings settings;

    settings.useDhcp = int(ui->dhcpCB->isChecked());
    settings.newAddressString = ui->newIpLineEdit->text().trimmed();
    settings.newPort = ui->newPortSpinBox->value();
    settings.netmask = ui->netmaskLineEdit->text().trimmed();
    settings.gateway = ui->gateLineEdit->text().trimmed();

    emit updateNetworkSettings(settings);
}

void MainWindow::emitChangeSetpoints(){
    Setpoints setpoints;

    setpoints.loudThreshold = ui->loudThresholdSpinBox->value();
    setpoints.quietThreshold = ui->quietThresholdSpinBox->value();
    setpoints.loudTimeout = ui->loudTimeoutSpinBox->value();
    setpoints.quietTimeout = ui->quietTimeoutSpinBox->value();

    emit updateSetpoints(setpoints);
}

void MainWindow::emitPollingTimeChanged(int time){
    storedConnectionParams->setValue("POLLINGTIME", time);
    storedConnectionParams->sync();
    emit pollingTimeChanged(time);
}

void MainWindow::enableIpString(bool useDhcp){
    ui->newIpLineEdit->setEnabled(!useDhcp);
}

void MainWindow::changeToConnetedMode(){
    ui->deviceControlGB->setEnabled(true);
    ui->indication1GB->setEnabled(true);
    ui->indication2GB->setEnabled(true);
    ui->paramsGB->setEnabled(true);

    ui->connParamGB->setEnabled(false);
    setStatusText("Соединен");
}

void MainWindow::changeToDisconnectedMode(){
    ui->autoButton->setStyleSheet(passiveStyleSheet);
    ui->source1Button->setStyleSheet(passiveStyleSheet);
    ui->source2Button->setStyleSheet(passiveStyleSheet);

    ui->deviceControlGB->setEnabled(false);
    ui->indication1GB->setEnabled(false);
    ui->indication2GB->setEnabled(false);
    ui->paramsGB->setEnabled(false);

    ui->connParamGB->setEnabled(true);
    setStatusText("Разъединен");
}

void MainWindow::changeToConnectionMode(){
    changeToDisconnectedMode();
    ui->connParamGB->setEnabled(false);
    setStatusText("Соединение...");
}

void MainWindow::setState(State state){
    ui->source1LevelLeft->setValue(state.fstSourceLeft);
    ui->source1LevelRight->setValue(state.fstSourceRight);
    ui->source2LevelLeft->setValue(state.sndSourceLeft);
    ui->source2LevelRight->setValue(state.sndSourceRight);

    ui->autoButton->setStyleSheet(passiveStyleSheet);
    ui->source1Button->setStyleSheet(passiveStyleSheet);
    ui->source2Button->setStyleSheet(passiveStyleSheet);

    switch (state.currSource){
        case State::PRIM:
            ui->source1Button->setStyleSheet(activeStyleSheet);
            break;
        case State::SCND:
            ui->source2Button->setStyleSheet(activeStyleSheet);
            break;
        case State::AUTO:
            ui->autoButton->setStyleSheet(activeStyleSheet);
            break;

    }

    ui->relayRB->setChecked(bool(state.relayState));
    setStatusText("Соединен");
}

void MainWindow::setSetpoints(Setpoints setpoints){
     ui->loudThresholdSpinBox->setValue(setpoints.loudThreshold);
     ui->quietThresholdSpinBox->setValue(setpoints.quietThreshold);
     ui->loudTimeoutSpinBox->setValue(setpoints.loudTimeout);
     ui->quietTimeoutSpinBox->setValue(setpoints.quietTimeout);
     setStatusText("Новые параметры коммутации установлены");
}

void MainWindow::setNetworkSettings(NetworkSettings settings){
    ui->dhcpCB->setChecked(settings.useDhcp);
    ui->newIpLineEdit->setText(settings.newAddressString);
    ui->newPortSpinBox->setValue(settings.newPort);
    ui->netmaskLineEdit->setText(settings.netmask);
    ui->gateLineEdit->setText(settings.gateway);
    setStatusText("Новые параметры соединения установлены");

    storedConnectionParams->setValue("IPADDR", settings.newAddressString.trimmed());
    storedConnectionParams->setValue("PORT", settings.newPort);
    storedConnectionParams->sync();
}

void MainWindow::setStatusText(QString statusText){
    ui->connStateLabel->setText(statusText);
}

int MainWindow::getPollingTime(){
    return ui->pollingTimeSlider->value();
}
