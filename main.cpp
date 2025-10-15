/** @author Guilherme Martinelli Taglietti
 *  @file   main.cpp
 *  @brief  Application entry point
 */
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("QT Data Structure Playground");
    MainWindow w;
    w.show();
    return a.exec();
}
