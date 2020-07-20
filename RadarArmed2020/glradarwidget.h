#ifndef GLRADARWIDGET_H
#define GLRADARWIDGET_H

#include <QGLWidget>
#include <QMainWindow>

class GLRadarWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLRadarWidget(QWidget *parent = 0);
    ~GLRadarWidget();

    void setRect(QRect rect);

protected:
    void initializeGL();
    void paintGL();

private slots:
    void timeOut();

private:
    QRect region;
    QTimer *timer;

    void drawObjectPainter();


};
//! [3]

#endif
