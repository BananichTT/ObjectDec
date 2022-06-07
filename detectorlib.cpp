#include "detectorlib.h"
#include <iostream>
#include <QFile>

DetectorLib::DetectorLib()
{

}

int DetectorLib::input_data(const cv::Mat &image, methods m)
{
    cv::Mat img = image;
    int num_obj = det_img(img, m);
    return num_obj;
}

int DetectorLib::input_data(const std::string &image, methods m)
{
    cv::Mat img = cv::imread(image);
    int num_obj = det_img(img, m);
    return num_obj;
}

int DetectorLib::input_data(const std::vector<uint8_t> &buffer, methods m)
{
    cv::Mat img = cv::imdecode(cv::Mat(buffer), cv::IMREAD_UNCHANGED);
    int num_obj = det_img(img, m);
    return num_obj;
}

cv::Mat Decode(std::vector<uint8_t> &buff)
{
    cv::Mat img = imdecode(cv::Mat(buff), cv::IMREAD_UNCHANGED);
    return img;
}

void DetectorLib::parse_data(const std::string &m_fileName, methods m, frame_state state, image_saving_protocol &read_protocol)
{
    std::ifstream fs(m_fileName, std::ios::binary);
    switch(state)
    {
        case frame_state::next:{position += 2; break;}
        case frame_state::previos:{position -= 2; break;}
        default: {position += 0;}
    }
    int f = 0;
    fs.seekg(FrameByteIndex[position], std::ios_base::beg);
    while (f < 2)
    {
        int fff = 0;
        fs.read((char*)&fff,sizeof(unsigned int));
        fs.read((char*)&read_protocol.NUMBER_OF_FRAMES,sizeof (unsigned int));
        fs.read((char*)&read_protocol.tmsec,sizeof (uint64));
        int size;
        fs.read((char*)&size,sizeof(int));
        std::vector<uint8_t> buff1;
        buff1.resize(size);
        fs.read(reinterpret_cast<char*>(&buff1.front()),buff1.size());
        det_img(Decode(buff1),m);
        f++;
    }
}

std::pair<int, state> DetectorLib::add_template(const cv::Mat &templ_one) {
  cv::Mat tmp = templ_one;
  cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
  adaptiveThreshold(tmp, tmp, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 151,
                    100);
  templ_counter++;
  templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

  return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> DetectorLib::add_template(std::vector<cv::Mat> &templs) {
  std::vector<int> position; // возвращает позицию изображения
  for (const auto &img : templs) {
    cv::Mat tmp = img;
    cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
    templ_counter++;
    templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));
    position.push_back(templ_counter);
  }
  return std::pair<std::vector<int>, state>(position, state::success);
}

std::pair<int, state> DetectorLib::add_template(const std::string &templ_one) {
  cv::Mat tmp = cv::imread(templ_one);
  cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
  templ_counter++;
  templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

  return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> DetectorLib::add_template(std::vector<std::string> &templs) {
  std::vector<int> position; // возвращает позицию изображения
  for (const auto &file_name : templs) {
    cv::Mat tmp = cv::imread(file_name);
    cvtColor(tmp, tmp, cv::COLOR_BGR2GRAY);
    templ_counter++;
    templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));
    position.push_back(templ_counter);
  }
  return std::pair<std::vector<int>, state>(position, state::success);
}

QVector<cv::Point> tresh(double treshold, const cv::Mat &result) {
  QVector<cv::Point> vec;

  cv::Mat_<uchar> result1 = result;
  cv::Mat test(result.size(), CV_8UC1);
  for (int i = 0; i < result1.rows; i++) {

    for (int j = 0; j < result1.cols; j++) {
      uchar a = static_cast<uchar>((result.at<float>(i,j) * 127) -255);
      test.at<uchar>(i,j) = a;
      if (result.at<float>(i, j) > treshold) {

        vec.push_back(cv::Point(j, i));
      }
    }
  }
  adaptiveThreshold(test, test, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101,
                    15);
  return vec;
}

void draw_rect(cv::Mat &qimg, const QVector<QVector<cv::Point>> point_mas){
    for(int i = 0; i < point_mas.length(); i++){
        for(int j = 0; j < point_mas[i].length(); j++){
            rectangle(qimg, cv::Point(point_mas[i][j].x, point_mas[i][j].y),
                      cv::Point(point_mas[i][j].x + 50, point_mas[i][j].y + 50), cv::Scalar(0,0,0),
                      2);          
        }
    }
}

cv::Mat convert_to_CV_8UC1(const cv::Mat &result) {
  cv::Mat_<uchar> result1 = result;
  cv::Mat test(result.size(), CV_8UC1);
  for (int i = 0; i < result1.rows; i++) {
    for (int j = 0; j < result1.cols; j++) {
      uchar a = static_cast<uchar>((result.at<float>(i, j) * 127) + 255);
      test.at<uchar>(i, j) = a;
    }
  }
  cv::imshow("res", test);
  return test;
};

QVector<cv::Point> find_conturs(cv::Mat &input_img){
    adaptiveThreshold(input_img, input_img, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101,
                      15);

    cv::Mat canny_output;
    cv::Canny(input_img, canny_output, 100, 100*2 );
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );

    std::vector<std::vector<cv::Point>> hull(contours.size());
    for(unsigned int i = 0,n = contours.size(); i < n; ++i) {
        cv::convexHull(cv::Mat(contours[i]),hull[i],false);
    }
    cv::Mat contours_result = cv::Mat::zeros(input_img.size(), CV_8UC3);

    QVector<cv::Point> point;

    for (unsigned int i = 0, n = contours.size(); i < n; ++i) {
        if(contours[i].size() > 10){
            cv::RotatedRect rect = cv::fitEllipse(cv::Mat(contours[i]));
            point.push_back(rect.center);
        }

    }

    return point;
};

void delete_rect(QVector<cv::Point> &point){
    QVector<cv::Point>::iterator iterator;
    if(point.length() > 1){
    for(iterator = point.begin(); iterator != point.end(); iterator++){
       int dx = abs(iterator->x - (iterator + 1)->x);
       int dy = abs(iterator->y - (iterator + 1)->y);
       if(dx < 10 || dy < 10){
           iterator = point.erase(iterator);

       }
    }
  }
}

int DetectorLib::det_img(const cv::Mat &image, methods m)
{
  QVector<QVector<cv::Point>> point_mas;
  QVector<cv::Point> points;

  cv::Mat img = image;
  int match_method = m;
  cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

  adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_MEAN_C,
                    cv::THRESH_BINARY, 151, 100);

  std::map<int, cv::Mat>::iterator iterator;
  for (iterator = templ.begin(); iterator != templ.end(); iterator++) {

    cv::Mat res_32f(img.rows - iterator->second.rows + 1,
                    img.cols - iterator->second.cols + 1, 0);
    matchTemplate(img, iterator->second, res_32f, match_method);
    cv::imshow("do conv", res_32f);
    cv::Mat convert = convert_to_CV_8UC1(res_32f);
    points = find_conturs(convert);
    delete_rect(points);
    point_mas.append(points);
  }
  draw_rect(img, point_mas);

  cv::imshow("IMAGE", img);
  return points.length();
}

std::vector<uint64> DetectorLib::Data_Indexing(const QString &m_fileName)
{
    std::vector<uint64> indexes;
    std::ifstream fs(m_fileName.toStdString(), std::ios::binary);
    QFile file(m_fileName);
    if(file.exists())
    {
        image_saving_protocol read_protocol;
        uint64 def_size = 0;
        indexes.push_back(def_size);
        while(!fs.eof())
        {
            fs.read((char*)&read_protocol.CAMERA_ID,sizeof(unsigned int));
            fs.read((char*)&read_protocol.NUMBER_OF_FRAMES,sizeof (unsigned int));
            fs.read((char*)&read_protocol.tmsec,sizeof (uint64));
            int size;
            fs.read((char*)&size,sizeof(int));
            std::vector<uint8_t> buff1;
            buff1.resize(size);
            fs.read(reinterpret_cast<char*>(&buff1.front()),buff1.size());
            def_size += size;
            def_size += 20;
            indexes.push_back(def_size);
        }
    }
    return indexes;
}

