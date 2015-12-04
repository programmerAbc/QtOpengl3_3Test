#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"
#include "refreshthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   void run();
private:
    Ui::MainWindow *ui;
    OpenglWidget* openglWidget;
    RefreshThread* refreshThread;

};

#endif // MAINWINDOW_H
