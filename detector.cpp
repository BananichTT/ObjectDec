#include <detector.h>
#include <iostream>
detector::detector(int a)
{
    cv::Mat img = cv::imread("D:/borig.jpg");
    cv::Mat templ = cv::imread("D:/b2.jpg");
    std::cout<<"dsaghjh";
    add_template(templ);
    input_data(img, methods::CCOEFF_NORMED);
}

void detector::input_data(const cv::Mat &image, methods m)
{
    cv::Mat img = image;
    det_img(image, m);
    std::cout<<"fdsshs";
}

void detector::input_data(const std::vector<uint8_t> &buffer, methods m)
{
    cv::Mat img = cv::imdecode(cv::Mat(buffer), cv::IMREAD_UNCHANGED);
    det_img(img, m);
}

std::pair<int, state> detector::parse_data(const std::string &file, methods m)
{

}

std::pair<int, state> detector::add_template(const cv::Mat &templ_one)
{
    cv::Mat tmp = templ_one;
    templ_counter ++;
    templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> detector::add_template(std::vector<cv::Mat> &templs)
{
    std::vector<int> position; // возвращает позицию изображения
    for (const auto &img:templs){
        cv::Mat tmp = img;
        templ_counter ++;
        templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));
        position.push_back(templ_counter);
    }
    return std::pair<std::vector<int>, state>(position, state::success);
}

std::pair<int, state> detector::add_template(std::string &templ_one)
{
        cv::Mat tmp = cv::imread(templ_one);
        templ_counter ++;
        templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

    return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> detector::add_template(std::vector<std::string> &templs)
{
    std::vector<int> position; // возвращает позицию изображения
    for (const auto &file_name:templs){
        cv::Mat tmp = cv::imread(file_name);
        templ_counter ++;
        templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));
        position.push_back(templ_counter);
    }
    return std::pair<std::vector<int>, state>(position, state::success);
}

QVector<cv::Point> tresh(double treshold, const cv::Mat &result) {
  QVector<cv::Point> vec;

  cv::Mat_<uchar> result1 = result;

  for (int i = 0; i < result1.rows; i++) {

    for (int j = 0; j < result1.cols; j++) {

      // qDebug()<<result.at<float>(i,j);
      if (result.at<float>(i, j) > treshold) {

        vec.push_back(cv::Point(j, i));
      }
    }
  }
  return vec;
}

void delete_point(QVector<cv::Point> &q){

    QVector<cv::Point>::Iterator iterator;
    QVector<cv::Point>::Iterator iterator2;

    for (iterator = q.begin(); iterator != q.end(); iterator++) {
      for (iterator2 = iterator + 1; iterator2 != q.end();) {
        double dist =
            sqrt((iterator->x - iterator2->x) * (iterator->x - iterator2->x) +
                 (iterator->y - iterator2->y) * (iterator->y - iterator2->y));
        if (dist < 25) {
          iterator2 = q.erase(iterator2);
        } else {
          iterator2++;
        }
      }
    }
};

void draw_rect(cv::Mat &qimg, const QVector<QVector<cv::Point>> point_mas){
    for(int i = 0; i < point_mas.length(); i++){
        for(int j = 0; j < point_mas[i].length(); j++){
            rectangle(qimg, cv::Point(point_mas[i][j].x, point_mas[i][j].y),
                      cv::Point(point_mas[i][j].x + 50, point_mas[i][j].y + 50), cv::Scalar(0, 255, 0),
                      2);
        }
    }
};

void detector::det_img(const cv::Mat &image, methods m)
{
    QVector<QVector<cv::Point>> point_mas;

    cv::Mat img = image;
    int match_method = m;
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101,
                      15);


    std::map<int, cv::Mat>::iterator iterator;

    for(iterator = templ.begin(); iterator != templ.end(); iterator++){
        cvtColor(iterator->second, iterator->second, cv::COLOR_BGR2GRAY);
        adaptiveThreshold(iterator->second, iterator->second, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101, 15);

        cv::Mat res_32f(img.rows - iterator->second.rows + 1, img.cols - iterator->second.cols + 1, CV_8UC1);
        matchTemplate(img, iterator->second, res_32f, match_method);

        //find_conturs(res_32f);
        QVector<cv::Point> q = tresh(0.4, res_32f);
        delete_point(q);
        point_mas.push_back(q);
    }

    draw_rect(img, point_mas);
    std::cout << "csadca";
    cv::imshow("csda", img);

    cv::waitKey(0);
}

