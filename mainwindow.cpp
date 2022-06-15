#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QFile"
#include "sys/time.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    if(dbase.connectToDataBase("./base.db")) {
//            qDebug()<<"okay";
//            info nform;
//            nform.id = 3;
//            nform.img = QImage("./img.jpg");
//            gettimeofday(&nform.time, NULL);
//            nform.type = "imaga";
//            nform.coordinate = std::pair<double, double>(1.1, 6.5);
//            dbase.insertIntoTable(nform);


//        }
//        else {
//            qDebug()<<"not okay";
//        }
//    connect(&detector, &DetectorLib::output_data, this, &MainWindow::output);

    capture.open("D:/copter.mp4");
    detector.add_template("D:/copter_t.jpg");
    connect(&detector, &DetectorLib::Image, this, &MainWindow::image);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// add img
void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0,tr("Open Config"), "", tr("Config Files (*.jpg)"));
    ui->lineEdit->setText(file_name);
    int a = detector.input_data(file_name.toStdString(), methods::CCOEFF_NORMED);
    ui->lineEdit->setText(QString::number(a));
}

// add temolate
void MainWindow::on_pushButton_2_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0,tr("Open Config"), "", tr("Config Files (*.jpg)"));
    ui->lineEdit_2->setText(file_name);
    detector.add_template(file_name.toStdString());
}

void MainWindow::output(object_inf inf)
{
  QImage img(inf.templateMatching_result.data, inf.templateMatching_result.rows,
             inf.templateMatching_result.step, QImage::Format_Mono);
  cv::imshow("img", inf.templateMatching_result);
  info nform;
  nform.id = 1;
  nform.img = img;
  nform.time = inf.time;
  nform.type = inf.type;
  nform.coordinate = std::pair<double, double>(1.1, 6.5);
  dbase.insertIntoTable(nform);
}




void MainWindow::on_pushButton_3_clicked()
{
    QList <outputInfo> list;
    list = dbase.searchInfoId("1");
    QImage img = QImage::fromData(list.first().img, "JPG");
   img = img.convertToFormat(QImage::Format_RGB888);


    qDebug() << pm.size();
    std::cout << list.first().time<< std::endl;
    qDebug() << img.size();
    ui->label->setPixmap(pm.fromImage(img));
}

void MainWindow::image(cv::Mat frame)
{
    QImage qimg(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Grayscale8);
    QPixmap p = QPixmap::fromImage(qimg.rgbSwapped());
    ui->label->setPixmap(p.scaled(ui->label->width(), ui->label->height()));
    next1->start();
}


void MainWindow::on_pushButton_4_clicked()
{
    next1 = new QTimer();
    connect(next1, &QTimer::timeout, this, &MainWindow::next);
    next1->setInterval(33);
    next1->start();
}

void MainWindow::next()
{
    next1->stop();
    cv::Mat frame;
    cv::Mat frame1;
    if(capture.isOpened()){
        capture >> frame;
        capture >> frame1;
        if(frame.empty() == false){
            detector.input_data(frame, methods::CCOEFF_NORMED);
        }
        QImage qimg(frame1.data,frame1.cols,frame1.rows,frame1.step,QImage::Format_RGB888);
        QPixmap p = QPixmap::fromImage(qimg.rgbSwapped());
        ui->label_2->setPixmap(p.scaled(ui->label_2->width(), ui->label_2->height()));
    }
}

