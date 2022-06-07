#include "test.h"

test::test(QObject *parent)
{

}

void test::test1() // тест на колличество птиц
{
    DetectorLib det;
    det.add_template("D:/b2.jpg");
    QCOMPARE(det.input_data("D:/bird2.jpg", methods::CCOEFF_NORMED), 1);
}

void test::test2()
{

}
