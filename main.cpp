/*
 * Team             |   Chandler
 * Members          |   Braden Fiedel, Caden Erickson, Connor Blood, Josie Fiedel
 * Class            |   CS 3505
 * Project          |   A8: Sprint 2
 * Last modified    |   April 26, 2023
 */


#include "mainwindow.h"
#include <QApplication>


/**
 * @brief Application entry point
 *
 * @param argc -- unused
 * @param argv -- unused
 * @return exit state
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.show();
    return a.exec();
}
