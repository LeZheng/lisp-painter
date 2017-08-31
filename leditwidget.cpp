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
    this->scene = new GraphicsSelectScene;
    this->scene->setGraphicsView(this->ui->painterView);
    this->scene->setSceneRect(- this->ui->painterView->width()/2,
                              - this->ui->painterView->height()/2,
                              this->ui->painterView->width(),
                              this->ui->painterView->height());
    painterView->setScene(scene);
    painterView->move(this->ui->tabWidget->x(),this->ui->tabWidget->y());
    painterView->setVisible(false);
    connect(this->scene,&GraphicsSelectScene::rectSelected,this,&LEditWidget::chooseRectText);
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
            this->ui->painterView->raise();
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
//        this->ui->painterView->resize(edit->width(),edit->height());
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
        this->ui->painterView->setVisible(true);
    }
    QWidget::mousePressEvent(event);
}

void LEditWidget::chooseRectText(int x,int y,int h,int w)
{
    QString path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
    QTextEdit * edit = edits[path];
    QPoint p(x,y);
    p = edit->mapFromGlobal(p);
    x = p.x();
    y = p.y();
    if(edit != NULL)
    {
        QFontMetrics fm(edit->currentFont());
        int dh = fm.height();
        QTextBlock block = edit->document()->begin();
        QStringList list;
        for(int i = 0,height = 0;i < edit->document()->lineCount();i++,height+= dh)
        {
            if((height >= y && height <= y + h) || (height + dh > y && height + dh < y + h))
            {
                QString line = block.text();
                int start = -1,end = -1,j;
                for(j = 0;j < line.length();j++)
                {

                    if(fm.width(line,j) > x && start < 0)
                    {
                        start = j;
                        continue;
                    }
                    if(fm.width(line,j) > (x + w))
                    {
                        end  = j;
                        break;
                    }
                    if(start > 0 && j == (line.length() - 1))
                    {
                        end = j + 1;
                    }
                }
                if(start >= 0 && end >= 0)
                {
                    list << block.text().mid(start,end - start);
                }
                else
                {
                    list << "";
                }
            }
            block = block.next();
        }
        emit currentTextSelected(list);
    }
}
