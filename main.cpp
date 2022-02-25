#include "RegulatoryTemplate.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegulatoryTemplate w;
    w.show();
    
    return a.exec();
}
