#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setOrganizationName("Faustos Comp.");
    qApp->setApplicationName("Tabac");

    Widget w;
    w.show();

    return a.exec();
}
