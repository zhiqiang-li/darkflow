#include "mainwindow.h"
#include <QApplication>
#include <Magick++.h>

int main(int argc, char *argv[])
{
    Magick::InitializeMagick(argv[0]);
    Magick::ResourceLimits::memory(1<<30);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
