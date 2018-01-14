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
}

void Presenter::connectAvarkomSignals(){
    QObject::connect(avarkomPMS, SIGNAL(connected()),
                     this, SLOT(indicateConnection()));
    QObject::connect(avarkomPMS, SIGNAL(disconnected()),
                     this, SLOT(indicateDisconnection()));

    QObject::connect(avarkomPMS, SIGNAL(requestProcessingFinished(Request *)),
                     this, SLOT(handleExecutedRequest(Request *)));
    QObject::connect(avarkomPMS, SIGNAL(errorReport(QString)),
                     this, SLOT(reportError(QString)));
}

void Presenter::handleExecutedRequest(Request *executedRequest){
    switch (executedRequest->getCode()) {
    case REQUEST_CODE::GET_STATE:
        updateState(executedRequest->getAnswer());
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
    Request* stateRequest = new Request(REQUEST_CODE::GET_STATE);
    avarkomPMS->processRequest(stateRequest);
}

void Presenter::changeDeviceState(QString newState){
    REQUEST_CODE newStateCode;

    if (newState == "auto"){
        newStateCode = REQUEST_CODE::SET_AUTO;
    }
    else if (newState == "prim"){
        newStateCode = REQUEST_CODE::SET_PRIM;
    }
    else if (newState == "scnd"){
        newStateCode = REQUEST_CODE::SET_SCND;
    }
    else{
        return;
    }

    Request* stateRequest = new Request(newStateCode);
    avarkomPMS->processRequest(stateRequest);
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

void Presenter::updateState(QString deviceAnswer){
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
