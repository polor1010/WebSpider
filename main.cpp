#include "WebSpider.h"

#include <QApplication>
#include <iostream>
#include <QtGui>

int main(int argc, char * argv[])
{

    QApplication a(argc, argv);

    WebSpider loader;
    loader.start();

    return a.exec();
}
