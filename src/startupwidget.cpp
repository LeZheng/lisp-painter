#include "startupwidget.h"
#include <QDebug>

StartupWidget::StartupWidget(int c,QWidget *parent)
    : QWidget(parent),initCount(c)
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    this->gView = new QGraphicsView(this);
    this->gView->setObjectName("gView");
    this->gView->setStyleSheet("QGraphicsView#gView{border-image:url(:/startup)}");
    this->gScene = new QGraphicsScene(this);

    this->gScene->setSceneRect(-200,-200,400,250);
    for(int i = 0;i < 10;i++)
    {
        i1[i].setPos(rand()%400 - 200,rand()%250 - 125);
        this->gScene->addItem(&i1[i]);
    }

    this->gView->setScene(this->gScene);
    this->resize(400,250);
    this->progressBar = new QProgressBar(this);
    this->progressBar->setRange(0,100);
    layout->setMargin(0);
    layout->addWidget(this->gView);
    layout->addWidget(this->progressBar);
    resize(400,250 + this->progressBar->height());
    startTimer(60);
}

StartupWidget::~StartupWidget()
{

}

void StartupWidget::timerEvent(QTimerEvent *event)
{
    for(int i = 0;i < 10;i++)
    {
        i1[i].moveToNext();
    }
}

void StartupWidget::initStateChange(QString state)
{
    //TODO state
    static int count = 0;
    count++;
    this->progressBar->setValue(count * 100 / initCount);
    if(count == initCount)
    {
        this->close();
        emit finished();
    }
}
