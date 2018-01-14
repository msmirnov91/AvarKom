#include "mainwindow.h"
#include <QApplication>
#include "presenter.h"
#include "parser.h"
#include "avarkom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    Avarkom dev;
    Parser pars;
    Presenter p(&dev, &pars, &w, 5000);
    return a.exec();
}
