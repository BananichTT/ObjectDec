
#include <QCoreApplication>
#include <detector.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    detector d(10);


    return a.exec();
}
