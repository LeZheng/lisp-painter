#include "leditwidget.h"
#include "ui_leditwidget.h"
#include <QDebug>

LEditWidget::LEditWidget(QWidget *parent) :
    LCloneableWidget(parent),
    ui(new Ui::LEditWidget)
{
    ui->setupUi(this);
    connect(this->ui->tabWidget,&QTabWidget::tabBarDoubleClicked,
            [=](int index)
    {
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

    remindView = new QTableView();
    model = new QStandardItemModel();
    remindView->verticalHeader()->setVisible(false);
    remindView->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    remindView->setModel(model);
    connect(remindView,&QTableView::doubleClicked,
            [=](const QModelIndex &index)
    {
        QStandardItem * item = this->model->item(index.row());
        remindView->hide();
        QTextEdit * edit = edits.value(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
        QTextCursor cursor = edit->textCursor();
        if(!cursor.hasSelection())
        {
            cursor.select(QTextCursor::WordUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText(item->text());
            edit->setTextCursor(cursor);
        }
    });
}

LEditWidget::~LEditWidget()
{
    delete scene;
    delete ui;
}

LCloneableWidget * LEditWidget::clone()
{
    LEditWidget * widget = new LEditWidget();
    QListIterator<QString> iterator(edits.keys());
    while(iterator.hasNext())
    {
        widget->open(iterator.next());
    }
    return widget;
}

void LEditWidget::open(QString path)
{
    activateWindow();
    if(!edits.contains(path))
    {
        QFile f(path);
        if(f.open(QIODevice::ReadWrite))
        {
            QTextEdit * edit = new QTextEdit(this);
            edit->setLineWrapMode(QTextEdit::WidgetWidth);
            edit->setWordWrapMode(QTextOption::NoWrap);
            edit->installEventFilter(this);
            connect(edit,&QTextEdit::currentCharFormatChanged,this,&LEditWidget::currentCharFormatChanged);
            edits[path] = edit;
            edit->setContextMenuPolicy (Qt::NoContextMenu);
            this->ui->tabWidget->addTab(edit,path);
            this->ui->tabWidget->setCurrentWidget(edit);
            this->ui->painterView->raise();
            LispSyntaxHighlighter * shLighter = new LispSyntaxHighlighter(edit->document());
            LRegExpHighlighterStrategy * s1 = new LRegExpHighlighterStrategy(KEY_WORD_ARGU,shLighter);
            s1->setForeground(Qt::yellow);
            LRegExpHighlighterStrategy * s2 = new LRegExpHighlighterStrategy(STRING_WORD,shLighter);
            s2->setForeground(Qt::darkRed);
            LRegExpHighlighterStrategy * s3 = new LRegExpHighlighterStrategy(ARGU_TYPE_WORD,shLighter);
            s3->setForeground(Qt::darkCyan);
            LSymbolHighlighterStrategy * s4 = new LSymbolHighlighterStrategy(shLighter);
            s4->setFontWeight(QFont::Bold);
            s4->setForeground(Qt::darkMagenta);
            edit->setText(QString(f.readAll().data()));
            QWidget::connect(edit,&QTextEdit::textChanged,
                [=]()
            {
                int index = this->ui->tabWidget->currentIndex();
                if(this->ui->tabWidget->tabIcon(index).isNull()){
                    QSize size(200,200);
                    QImage img(size,QImage::Format_ARGB32);
                    QPainter painter(&img);
                    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
                    QPen pen = painter.pen();
                    pen.setColor(Qt::red);
                    QFont font = painter.font();
                    font.setBold(true);
                    font.setPixelSize(200);
                    painter.setPen(pen);
                    painter.setFont(font);
                    painter.drawText(img.rect(),Qt::AlignCenter,"*");
                    QIcon icon(QPixmap::fromImage(img));
                    this->ui->tabWidget->setTabIcon(index,icon);
                }
            });
        }
        else
        {
            qWarning() << "file open failed";
        }
        f.close();
    }
    else
    {
        int i = this->ui->tabWidget->indexOf(edits[path]);
        this->ui->tabWidget->setCurrentIndex(i);
    }
}

void LEditWidget::save(QString path)
{
    activateWindow();
    if(path == "")
        path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
    QTextEdit * edit = edits[path];
    if(edit != NULL){
        QFile f(path);
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(edit->toPlainText().toUtf8());
            f.close();
            this->ui->tabWidget->setTabIcon(this->ui->tabWidget->currentIndex(),QIcon());
        }
        else
        {
            qWarning() << "file save failed";
        }
    }
}

void LEditWidget::create(QString path)
{
    activateWindow();
    QFile f(path);
    if(f.open(QIODevice::ReadWrite))
    {
        QTextEdit * edit = new QTextEdit(this);
        edit->setLineWrapMode(QTextEdit::WidgetWidth);
        edit->setWordWrapMode(QTextOption::NoWrap);
        edit->setContextMenuPolicy (Qt::NoContextMenu);
        edits[path] = edit;
        this->ui->tabWidget->addTab(edit,path);
        //TODO
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
    LCloneableWidget::mousePressEvent(event);
}

void LEditWidget::chooseRectText(int x,int y,int h,int w)
{
    y -= 4;//TODO this value have not get from qt api
    x -= 5;//TODO this value have not get from qt api
    QString path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
    QTextEdit * edit = edits[path];
    QPoint p(x,y);
    p = edit->mapFromGlobal(p);
    x = p.x();
    y = p.y();
    if(edit != NULL)
    {
        double proc = edit->verticalScrollBar()->value() * 1.0 / edit->verticalScrollBar()->maximum();
        QFontMetrics fm(edit->currentFont());
        int dh = fm.height();
        QTextBlock block = edit->document()->begin();
        QStringList list;
        y = y + (dh * edit->document()->lineCount() - edit->height()) * proc;

        for(int i = 0,height = 0;i < edit->document()->lineCount();i++,height+= dh)
        {
            if((height >= y && height <= y + h) || (height + dh >= y && height + dh <= y + h))
            {
                QString line = block.text();
                int start = -1,end = -1,j;
                for(j = 0;j < line.length();j++)
                {
                    if(fm.width(line,j + 1) > x && start < 0)
                    {
                        start = j;
                        continue;
                    }
                    if(fm.width(line,j + 1) > (x + w))
                    {
                        end  = j;
                        break;
                    }
                    if(start >= 0 && j == (line.length() - 1))
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
        QString temp = "";
        foreach (QString str, list) {
            temp.append(str);
            temp.append("\n");
        }
        QMessageBox::information(this,tr("select text"),temp,QMessageBox::Yes);
        emit currentTextSelected(list);
    }
}

void LEditWidget::selectCurrentWord()
{
    if(edits.contains(ui->tabWidget->tabText(ui->tabWidget->currentIndex())))
    {
        QTextEdit * edit = edits.value(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
        QTextCursor cursor = edit->textCursor();
        if(!cursor.hasSelection())
        {
            cursor.select(QTextCursor::WordUnderCursor);
            QString word = cursor.selectedText();
            LispSymbolFactory * lsf = LispSymbolFactory::getInstance();
            QList<LispSymbol *> symbols = lsf->getSymbols(word);//TODO need delete

            model->clear();
            foreach (LispSymbol * sym, symbols) {
                QList<QStandardItem *> list;
                list.append(new QStandardItem(sym->name()));//TODO need delete
                list.append(new QStandardItem(sym->type()));
                model->insertRow(0,list);
            }
            QPoint p = QCursor::pos();
            remindView->move(p.x(),p.y());
            remindView->show();
        }
    }
}

void LEditWidget::changeFontType(QString font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    mergeFormat(fmt);
}

void LEditWidget::changeFontSize(QString value)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(value.toFloat());
    mergeFormat(fmt);
}
void LEditWidget::changeToBold(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    mergeFormat(fmt);
}
void LEditWidget::changeToItalic(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    mergeFormat(fmt);
}
void LEditWidget::changeToUnderline(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    mergeFormat(fmt);
}
void LEditWidget::changeColor()
{

    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        mergeFormat(fmt);
    }
}

