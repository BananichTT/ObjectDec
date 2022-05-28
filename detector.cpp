#include <detector.h>
#include <iostream>
#include <QFile>

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

cv::Mat Decode(std::vector<uint8_t> buff, int camera_id)
{
    cv::Mat img = imdecode(cv::Mat(buff), cv::IMREAD_UNCHANGED);
    return img;

}

std::pair<int, state> detector::parse_data(const std::string &m_fileName, methods m, frame_state state, image_saving_protocol &read_protocol)
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
        det_img(Decode(buff1,fff),m);
        f++;
    }
}

std::pair<int, state> detector::add_template(const cv::Mat &templ_one) {
  cv::Mat tmp = templ_one;
  templ_counter++;
  templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

  return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> detector::add_template(std::vector<cv::Mat> &templs) {
  std::vector<int> position; // возвращает позицию изображения
  for (const auto &img : templs) {
    cv::Mat tmp = img;
    templ_counter++;
    templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));
    position.push_back(templ_counter);
  }
  return std::pair<std::vector<int>, state>(position, state::success);
}

std::pair<int, state> detector::add_template(std::string &templ_one) {
  cv::Mat tmp = cv::imread(templ_one);
  templ_counter++;
  templ.insert(std::pair<int, cv::Mat>(templ_counter, tmp));

  return std::pair<int, state>(templ_counter, state::success);
}

std::pair<std::vector<int>, state> detector::add_template(std::vector<std::string> &templs) {
  std::vector<int> position; // возвращает позицию изображения
  for (const auto &file_name : templs) {
    cv::Mat tmp = cv::imread(file_name);
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

      //qDebug()<<result.at<float>(i,j);
      uchar a = static_cast<uchar>((result.at<float>(i,j) * 127) +255);
      test.at<uchar>(i,j) = a;
      if (result.at<float>(i, j) > treshold) {

        vec.push_back(cv::Point(j, i));
      }
    }
  }
  adaptiveThreshold(test, test, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 101,
                    15);
  cv::imshow("test", test);
  cv::waitKey(0);
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

std::vector<uint64> detector::Data_Indexing(const QString &m_fileName)
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

