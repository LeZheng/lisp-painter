#include "leditwidget.h"
#include "ui_leditwidget.h"
#include <QDebug>

LEditWidget::LEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LEditWidget)
{
    ui->setupUi(this);
    connect(this->ui->tabWidget,&QTabWidget::tabBarDoubleClicked,
            [=](int index)
    {
        qDebug() << this->ui->tabWidget->tabText(index);
        this->edits.remove(this->ui->tabWidget->tabText(index));
        QWidget * w = this->ui->tabWidget->widget(index);
        this->ui->tabWidget->removeTab(index);
        delete w;
    });

    connect(this->ui->tabWidget,&QTabWidget::currentChanged,
            [=](int index)
    {
        QString path = this->ui->tabWidget->tabText(index);
        emit currentFileChanged(path);
    });

    QGraphicsView * painterView = this->ui->painterView;
    painterView->setStyleSheet("background: transparent;border:0px");
    this->scene = new QGraphicsScene;
    this->scene->setSceneRect(- this->ui->painterView->width()/2,
                              - this->ui->painterView->height()/2,
                              this->ui->painterView->width(),
                              this->ui->painterView->height());
    painterView->setScene(scene);
    painterView->move(this->ui->tabWidget->x(),this->ui->tabWidget->y());
    painterView->setVisible(false);
    this->ui->painterView->installEventFilter(this);
//    this->setMouseTracking(true);
}

LEditWidget::~LEditWidget()
{
    delete ui;
}

void LEditWidget::open(QString path)
{
    if(!edits.contains(path))
    {
        QFile f(path);
        if(f.open(QIODevice::ReadWrite))
        {
            QTextEdit * edit = new QTextEdit(this);
            connect(edit,&QTextEdit::currentCharFormatChanged,this,&LEditWidget::currentCharFormatChanged);
            edits[path] = edit;
            edit->setContextMenuPolicy (Qt::NoContextMenu);
            this->ui->tabWidget->addTab(edit,path);
            this->ui->tabWidget->setCurrentWidget(edit);
            edit->setText(QString(f.readAll().data()));
        }
        else
        {
            //TODO open failed
        }
    }
    else
    {
        int i = this->ui->tabWidget->indexOf(edits[path]);
        this->ui->tabWidget->setCurrentIndex(i);
    }
}

void LEditWidget::save(QString path)
{
    QTextEdit * edit = edits[path];
    if(edit != NULL){
        QFile f(path);
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(edit->toPlainText().toUtf8());
            f.close();
        }
        else
        {
            //TODO
        }
    }
}

void LEditWidget::create(QString path)
{
    QFile f(path);
    if(f.open(QIODevice::ReadWrite))
    {
        QTextEdit * edit = new QTextEdit(this);
        edit->setContextMenuPolicy (Qt::NoContextMenu);
        edits[path] = edit;
        this->ui->tabWidget->addTab(edit,path);
    }
}

void LEditWidget::mergeFormat(QTextCharFormat fmt)
{

    QMapIterator<QString,QTextEdit *> iterator(edits);
    while(iterator.hasNext())
    {
        QTextEdit * text = iterator.next().value();
        QTextCursor cursor = text->textCursor();
        if(!cursor.hasSelection())
        {
            cursor.select(QTextCursor::WordUnderCursor);
        }
        cursor.mergeCharFormat(fmt);
        text->mergeCurrentCharFormat(fmt);
    }
}
void LEditWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QList<QGraphicsItem *> items = scene->items();
        while(!items.isEmpty())
        {
            scene->removeItem(items.at(0));
            items.removeAt(0);
        }
        this->ui->painterView->setVisible(true);
    }
    this->startx = event->x();
    this->starty = event->y();
    QWidget::mousePressEvent(event);
}
void LEditWidget::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent x:" << event->x() << " y:" << event->y();
//TODO
    if(event->button() == Qt::RightButton)
    {
        this->ui->painterView->setVisible(false);
    }
    QWidget::mouseReleaseEvent(event);
}

void LEditWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoveEvent x:" << event->x() << " y:" << event->y();
    if(this->ui->painterView->isVisible())
    {
        QList<QGraphicsItem *> items = scene->items();
        while(!items.isEmpty())
        {
            scene->removeItem(items.at(0));
            items.removeAt(0);
        }
        QGraphicsRectItem * item = new QGraphicsRectItem(QRectF(startx- this->ui->painterView->width()/2,
                                                                starty- this->ui->painterView->height()/2,
                                                                event->x() - startx,
                                                                event->y() - starty));
        item->setBrush(Qt::red);
        scene->addItem(item);
        item->setPos(0,0);
    }
    QWidget::mouseMoveEvent(event);
}

bool LEditWidget::eventFilter(QObject *watched, QEvent *event)
{
    qDebug() << "filter.." << event->type();
//TODO

    return QWidget::eventFilter(watched,event);
}
