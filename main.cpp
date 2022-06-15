#include "mainwindow.h"
#include <QTest>
#include "test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //QTest::qExec(new test, argc, argv);
    return a.exec();
}
