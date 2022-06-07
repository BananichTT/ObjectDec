#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QFile"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

