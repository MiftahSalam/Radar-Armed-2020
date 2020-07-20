#include <QtOpenGL>

#include "glradarwidget.h"

GLRadarWidget::GLRadarWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    if(!mask().isEmpty())
        region = mask().rects().first();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOut()));
    timer->setInterval(1000);
    timer->start();
}

void GLRadarWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

}

void GLRadarWidget::setRect(QRect rect)
{
    region = rect;
}

void GLRadarWidget::timeOut()
{
//    drawObjectPainter();
    updateGL();

    qDebug()<<Q_FUNC_INFO<<region;
}
/*
void GLRadarWidget::resizeEvent(QResizeEvent *)
{

}
*/
void GLRadarWidget::paintGL()
{
    QPixmap bufPixmap=QPixmap(region.width(),region.height());
    bufPixmap.fill(Qt::transparent);

    QPainter bufPaint(this);
    bufPaint.setRenderHint(QPainter::Antialiasing);
    bufPaint.setPen(Qt::SolidLine);
    bufPaint.setPen(QColor(192,128,0,255));
    bufPaint.translate((region.width())/2,(region.height()/2));

    bufPaint.drawLine(0,0,200,200);
    /*
      compass scale text
    int range = qMin(region.width(),region.height())/4;
    QString text;
    for(int j=0;j<12;j++)
    {
        if(j<9)
            QTextStream(&text)<<(j*30)+90;
        else
            QTextStream(&text)<<(j*30)-270;

        QRect rect((range+20)*cos((j*30)*M_PI/180)-15,((range+20)*sin((j*30)*M_PI/180)-5),30,15);
        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter);
        QFont font;

        font.setPixelSize(12);
        bufPaint.setFont(font);
        bufPaint.drawText(rect,text,opt);
        //        bufPaint.drawRect(rect);
        text.clear();
    }

    bufPaint.rotate(270);
    */

}

void GLRadarWidget::drawObjectPainter()
{
}

GLRadarWidget::~GLRadarWidget()
{
}
