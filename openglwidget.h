#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>


#include<sys/time.h>
#define TIME(i,body) {                                                      \
    struct timeval t1, t2; double elapsed;                                  \
    gettimeofday(&t1, NULL);                                                \
    body                                                                    \
    gettimeofday(&t2, NULL);                                                \
    elapsed = (t2.tv_usec - t1.tv_usec);                                    \
    printf("performance counter:%s\n use time(us): %f\n",#i, elapsed); }

#define __PASS__ qDebug()<<__FILE__<<","<<__LINE__;


class OpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit OpenglWidget(QWidget *parent = 0);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};

#endif // OPENGLWIDGET_H
