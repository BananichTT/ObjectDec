#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<detectorlib.h>
#include <libdb.h>
#include <QPixmap>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void output(object_inf inf);
    void image(cv::Mat frame);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();



    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void next();

private:
    Ui::MainWindow *ui;
    DetectorLib detector;
    LibDB dbase;
    QPixmap pm;
    QTimer *next1;

    cv::VideoCapture capture;
};
#endif // MAINWINDOW_H
