#ifndef LEDITWIDGET_H
#define LEDITWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTextBlock>
#include <QScrollBar>
#include "graphicsselectscene.h"
#include "lispsyntaxhighlighter.h"

namespace Ui {
class LEditWidget;
}

class LEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LEditWidget(QWidget *parent = 0);
    ~LEditWidget();

signals:
    void currentFileChanged(QString path);
    void currentCharFormatChanged(const QTextCharFormat & fmt);
    void currentTextSelected(QStringList strs);
public slots:
    void open(QString path);
    void save(QString path);
    void create(QString path);
    void mergeFormat(QTextCharFormat fmt);
    void chooseRectText(int x,int y,int h,int w);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
private:
    Ui::LEditWidget *ui;
    QMap<QString,QTextEdit *> edits;
    GraphicsSelectScene * scene;
};

#endif // LEDITWIDGET_H
