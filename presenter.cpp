#include "presenter.h"

Presenter::Presenter(Avarkom *device, MainWindow *UIView){
    avarkomPMS = device;
    view = UIView;

    connectViewSignals();
    connectAvarkomSignals();

    pollingTimer = new QTimer();
    setPollingTime(view->getPollingTime());
    watchDog = new QTimer();
    watchDog->setInterval(5000);  // 5 sec timeout for connection

    connect(pollingTimer, SIGNAL(timeout()), this, SLOT(requestState()));
    connect(watchDog, SIGNAL(timeout()), this, SLOT(connectionTimeout()));

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
    QObject::connect(view, SIGNAL(toggleRelay(bool)),
                     this, SLOT(toggleRelay(bool)));
    QObject::connect(view, SIGNAL(pollingTimeChanged(int)),
                     this, SLOT(setPollingTime(int)));
    QObject::connect(view, SIGNAL(updateSetpoints(Setpoints)),
                     this, SLOT(updateDeviceSetpoints(Setpoints)));
    QObject::connect(view, SIGNAL(updateNetworkSettings(NetworkSettings)),
                     this, SLOT(updateDeviceNetworkSettings(NetworkSettings)));
}

void Presenter::connectAvarkomSignals(){
    QObject::connect(avarkomPMS, SIGNAL(connected()),
                     this, SLOT(onConnect()));
    QObject::connect(avarkomPMS, SIGNAL(disconnected()),
                     this, SLOT(onDisconnect()));
    QObject::connect(avarkomPMS, SIGNAL(requestProcessingFinished(Command *)),
                     this, SLOT(handleExecutedRequest(Command *)));
    QObject::connect(avarkomPMS, SIGNAL(errorReport(QString)),
                     this, SLOT(reportError(QString)));
}

void Presenter::handleExecutedRequest(Command *executedRequest){
    if (executedRequest->isAnswered()){
        switch (executedRequest->getCode()) {
        case COMMAND_CODE::STATE: {
            State currentAvarkomState;
            Parser::parseState(currentAvarkomState, executedRequest->getAnswer());
            view->setState(currentAvarkomState);
            break;
            }
        case COMMAND_CODE::COMMUTATION_STATE: {
            Setpoints currentSetpoints;
            Parser::parseSetpoints(currentSetpoints, executedRequest->getAnswer());
            view->setSetpoints(currentSetpoints);
            break;
            }
        case COMMAND_CODE::NETWORK_STATE: {
            NetworkSettings currentNetworkSettings;
            Parser::parseNetworkSettings(currentNetworkSettings, executedRequest->getAnswer());
            view->setNetworkSettings(currentNetworkSettings);
            break;
            }
        default:
            break;
        }
    }
    else{
        avarkomPMS->abortConnection();
        view->changeToDisconnectedMode();
        view->setStatusText("Превышен таймаут ожидания ответа");
    }
    delete executedRequest;
}

void Presenter::reportError(QString error){
    view->setStatusText(error);
}

void Presenter::requestState(){
    Command* stateRequest = new Command(COMMAND_CODE::STATE);
    avarkomPMS->processNewCommand(stateRequest);
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

void Presenter::toggleRelay(bool state){
    int integerState = int(state);
    integerState &= 1;  // make it 0 or 1

    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::RELAY, integerState));
}

void Presenter::updateDeviceSetpoints(Setpoints setpoints){
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::LOUD_THR, setpoints.loudThreshold));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::QUIET_THR, setpoints.quietThreshold));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::LOUD_TIMEOUT, setpoints.loudTimeout));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::QUIET_TIMEOUT, setpoints.quietTimeout));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::COMMUTATION_STATE));
}

void Presenter::updateDeviceNetworkSettings(NetworkSettings settings){
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::DHCP, settings.useDhcp));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::IP_ADDR, settings.newAddressString));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::PORT, settings.newPort));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::NETMASK, settings.netmask));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::GATEWAY, settings.gateway));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::NETWORK_STATE));
}

void Presenter::makeConnecton(){
    QHostAddress addr;
    addr.setAddress(view->getAddressString());

    avarkomPMS->connect(addr,view->getPort());
    view->changeToConnectionMode();
    watchDog->start();
}

void Presenter::makeDisconnecton(){
    avarkomPMS->disconnect();
    pollingTimer->stop();
    watchDog->stop();
}

void Presenter::connectionTimeout(){
    avarkomPMS->abortConnection();
    view->changeToDisconnectedMode();
    watchDog->stop();
    reportError("Превышено время ожидания соединения");
}

void Presenter::setPollingTime(int pollingTime){
    pollingTimer->setInterval(pollingTime);
}

void Presenter::onConnect(){
    watchDog->stop();
    view->changeToConnetedMode();
    requestAllParams();
    pollingTimer->start();
}

void Presenter::onDisconnect(){
    view->changeToDisconnectedMode();
}

void Presenter::requestAllParams(){
    requestState();
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::COMMUTATION_STATE));
    avarkomPMS->processNewCommand(new Command(COMMAND_CODE::NETWORK_STATE));
}

