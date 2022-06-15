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
    void number_birds_test_string();
    void number_birds_test_img();
    void number_birds_test_buffer();
    void add_template_string_test();
    void add_template_img_test();
    void add_templates_strings_test();
    void add_template_imgs_test();
    void number_copter_test_img();

    void video_test();

};

#endif // TEST_H
