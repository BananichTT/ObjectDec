#include "test.h"

test::test(QObject *parent)
{

}
void test::number_birds_test_img()
{
    DetectorLib det;
    det.add_template("D:/b2.jpg");
    cv::Mat img = cv::imread("D:/bird2.jpg");
    QCOMPARE(det.input_data(img, methods::CCOEFF_NORMED), 1);
}

void test::number_birds_test_buffer()
{
    DetectorLib det;
    det.add_template("D:/b2.jpg");
    cv::Mat img = cv::imread("D:/bird2.jpg");

    std::vector<uint8_t> buff;
    std::vector<int> p;
    p.push_back(cv::IMWRITE_JPEG_QUALITY);
    p.push_back(5);
    cv::imencode(".jpg",img, buff, p);

    QCOMPARE(det.input_data(buff, methods::CCOEFF_NORMED), 1);
}

void test::number_birds_test_string()
{
    DetectorLib det;
    det.add_template("D:/b2.jpg");
    QCOMPARE(det.input_data("D:/bird2.jpg", methods::CCOEFF_NORMED), 1);
}

void test::add_template_string_test()
{
    DetectorLib det;
    QCOMPARE(det.add_template("D:/b2.jpg"), (std::pair<int,state>(1, state::success)));
}

void test::add_template_img_test()
{
    DetectorLib det;
    cv::Mat templ = cv::imread("D:/b2.jpg");
    QCOMPARE(det.add_template(templ), (std::pair<int,state>(1, state::success)));
}


void test::add_template_imgs_test()
{
    DetectorLib det;
    cv::Mat templ1 = cv::imread("D:/b1.jpg");
    cv::Mat templ2 = cv::imread("D:/b2.jpg");
    std::vector<cv::Mat> templs = {templ1, templ2};

    QCOMPARE(det.add_template(templs), (std::pair<std::vector<int>,state>(std::vector<int>{1,2}, state::success)));

}

void test::add_templates_strings_test()
{
    DetectorLib det;
    std::vector<std::string> templs = {"D:/b2.jpg","D:/b1.jpg"};
//    auto test = det.add_template(templs);
//    qDebug() << test.first.front();
//    qDebug() << test.first[1];
    QCOMPARE(det.add_template(templs), (std::pair<std::vector<int>,state>(std::vector<int>{1,2}, state::success)));
}

void test::number_copter_test_img()
{
    DetectorLib det;
    det.add_template("D:/copter_t.jpg");
    cv::Mat img = cv::imread("D:/copter.jpg");
    QCOMPARE(det.input_data(img, methods::CCOEFF_NORMED), 6);
}

void test::video_test()
{

}
