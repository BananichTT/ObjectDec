#ifndef TEST_H
#define TEST_H

#include <detectorlib.h>
#include <QTest>
#include <QObject>

class test:public QObject
{
    Q_OBJECT
public:
   explicit test(QObject *parent = 0);

private slots:
    void test1();
    void test2();
};

#endif // TEST_H
