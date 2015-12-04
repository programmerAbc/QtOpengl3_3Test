#include "refreshthread.h"

RefreshThread::RefreshThread(QOpenGLWidget *Widget)
{
    widget=Widget;
}

RefreshThread::~RefreshThread()
{

}

void RefreshThread::run()
{
    while(true)
    {
        widget->update();
        usleep(16000);
    }
}
