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
#include <QTableWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QColorDialog>
#include "graphicsselectscene.h"
#include "lispsyntaxhighlighter.h"
#include "lispsymbolfactory.h"
#include "lsplitcopywidget.h"

namespace Ui {
class LEditWidget;
}

class LEditWidget : public LCloneableWidget
{
    Q_OBJECT

public:
    explicit LEditWidget(QWidget *parent = 0);
    ~LEditWidget();
    virtual LCloneableWidget * clone();
signals:
    void currentFileChanged(QString path);
    void currentCharFormatChanged(const QTextCharFormat & fmt);
    void currentTextSelected(QStringList strs);
    void currentWordSelected(QString word);
public slots:

    void changeFontType(QString font);
    void changeFontSize(QString value);
    void changeToBold(bool checked);
    void changeToItalic(bool checked);
    void changeToUnderline(bool checked);
    void changeColor();

    void open(QString path);
    void save(QString path);
    void create(QString path);
    void mergeFormat(QTextCharFormat fmt);
    void chooseRectText(int x,int y,int h,int w);
    void selectCurrentWord();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
private:
    Ui::LEditWidget *ui;
    QMap<QString,QTextEdit *> edits;
    GraphicsSelectScene * scene;
    QTableView * remindView;
    QStandardItemModel * model;
};

#endif // LEDITWIDGET_H
