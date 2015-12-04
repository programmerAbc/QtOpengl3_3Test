#ifndef REFRESHTHREAD_H
#define REFRESHTHREAD_H
#include <QOpenGLWidget>
#include "posix_thread_wrapper/posixthreadwrapper.h"
class RefreshThread:public PosixThreadWrapper
{
private:
    QOpenGLWidget* widget;
public:
    RefreshThread(QOpenGLWidget* Widget);
    ~RefreshThread();
protected:
    void run();
};

#endif // REFRESHTHREAD_H
