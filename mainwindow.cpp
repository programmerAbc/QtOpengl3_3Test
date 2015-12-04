#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openglWidget=new OpenglWidget();
    openglWidget->showMaximized();
    refreshThread=new RefreshThread(openglWidget);
    refreshThread->start();
}




MainWindow::~MainWindow()
{
    delete ui;
}
