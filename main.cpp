#include "mainwindow.h"
#include <QApplication>
#include "presenter.h"
#include "parser.h"
#include "avarkom.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("PMS");
    QApplication::setApplicationName("AvarkomPMS");

    MainWindow w(QApplication::applicationDirPath());

    Avarkom dev;
    Parser pars;
    Presenter p(&dev, &pars, &w);
    return a.exec();
}
