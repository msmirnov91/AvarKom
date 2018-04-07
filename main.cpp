#include "mainwindow.h"
#include <QApplication>
#include "presenter.h"
#include "parser.h"
#include "avarkom.h"
#include "logger.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("PMS");
    QApplication::setApplicationName("AvarkomPMS");

    Logger::init(QApplication::applicationDirPath());
    Logger::setEnabled(false);

    // TODO: implement passing settings path in another way
    MainWindow w(QApplication::applicationDirPath());

    Avarkom dev;
    Presenter p(&dev, &w);
    return a.exec();
}
