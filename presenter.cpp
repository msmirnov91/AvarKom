#include "presenter.h"

Presenter::Presenter(Avarkom *device, Parser *parser, MainWindow *UIView,
                     int updateTime){
    avarkomPMS = device;
    responseParser = parser;
    view = UIView;

    connectViewSignals();
    connectAvarkomSignals();

    pollingTimer = new QTimer();
    connect(pollingTimer, SIGNAL(timeout()), this, SLOT(requestState()));
    pollingTime = updateTime;
    view->show();

}

Presenter::~Presenter(){
    pollingTimer->stop();
    avarkomPMS->disconnect();
    delete pollingTimer;
}

void Presenter::connectViewSignals(){
    QObject::connect(view, SIGNAL(connectionRequest()),
                     this, SLOT(makeConnecton()));
    QObject::connect(view, SIGNAL(disconnectionRequest()),
                     this, SLOT(makeDisconnecton()));
    QObject::connect(view, SIGNAL(changeStateRequest(QString)),
                     this, SLOT(changeDeviceState(QString)));
    QObject::connect(view, SIGNAL(updateSetpoints()),
                     this, SLOT(updateDeviceSetpoints()));
    QObject::connect(view, SIGNAL(updateNetworkSettings()),
                     this, SLOT(updateDeviceNetworkSettings()));
}

void Presenter::connectAvarkomSignals(){
    QObject::connect(avarkomPMS, SIGNAL(connected()),
                     this, SLOT(indicateConnection()));
    QObject::connect(avarkomPMS, SIGNAL(disconnected()),
                     this, SLOT(indicateDisconnection()));

    QObject::connect(avarkomPMS, SIGNAL(requestProcessingFinished(Command *)),
                     this, SLOT(handleExecutedRequest(Command *)));
    QObject::connect(avarkomPMS, SIGNAL(errorReport(QString)),
                     this, SLOT(reportError(QString)));
}

void Presenter::handleExecutedRequest(Command *executedRequest){
    switch (executedRequest->getCode()) {
    case COMMAND_CODE::STATE:
        updateViewState(executedRequest->getAnswer());
        break;
    default:
        qDebug() << "Received not state response";
        break;
    }

    delete executedRequest;
}

void Presenter::reportError(QString error){
    view->setErrorText(error);
}

void Presenter::requestState(){
    // Command* stateRequest = new Command(COMMAND_CODE::STATE);
    // avarkomPMS->processRequest(stateRequest);
}

void Presenter::changeDeviceState(QString newState){
    COMMAND_CODE newStateCode;

    if (newState == "auto"){
        newStateCode = COMMAND_CODE::SET_AUTO;
    }
    else if (newState == "prim"){
        newStateCode = COMMAND_CODE::SET_PRIM;
    }
    else if (newState == "scnd"){
        newStateCode = COMMAND_CODE::SET_SCND;
    }
    else{
        return;
    }

    Command* stateRequest = new Command(newStateCode);
    avarkomPMS->processNewCommand(stateRequest);
}

void Presenter::updateDeviceSetpoints(){
    // TODO: add all other parameters here!
    Command* loudTreshold = new Command(COMMAND_CODE::LOUD_THR,
                                        view->getLoudThreshold());
    avarkomPMS->processNewCommand(loudTreshold);
}

void Presenter::updateDeviceNetworkSettings(){
    // TODO: add all other parameters here!
    Command* newIp = new Command(COMMAND_CODE::IP_ADDR,
                                 view->getNewAddressString());

    Command* newPort = new Command(COMMAND_CODE::PORT,
                                   view->getNewPort());

    avarkomPMS->processNewCommand(newIp);
    avarkomPMS->processNewCommand(newPort);
}

void Presenter::makeConnecton(){
    QHostAddress addr;
    addr.setAddress(view->getAddressString());

    avarkomPMS->connect(addr,view->getPort());
    pollingTimer->start(pollingTime);
}

void Presenter::makeDisconnecton(){
    avarkomPMS->disconnect();
    pollingTimer->stop();
}

void Presenter::indicateConnection(){
    view->changeToConnetedMode();
}

void Presenter::indicateDisconnection(){
    view->changeToDisconnectedMode();
}

void Presenter::updateViewState(QString deviceAnswer){
    if (responseParser->parseState(deviceAnswer)){
        view->setState(responseParser->getState());

        view->set1SourceLeftLevel(responseParser->get1sourceLeftLevel());
        view->set1SourceRightLevel(responseParser->get1sourceRightLevel());
        view->set2SourceLeftLevel(responseParser->get2sourceLeftLevel());
        view->set2SourceRightLevel(responseParser->get2sourceRightLevel());
    }
    else{
        view->setErrorText("Неинтерпретируемый ответ устройства");
    }
}

