#ifndef LEDITWIDGET_H
#define LEDITWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

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
public slots:
    void open(QString path);
    void save(QString path);
    void create(QString path);
    void mergeFormat(QTextCharFormat fmt);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::LEditWidget *ui;
    QMap<QString,QTextEdit *> edits;
    QGraphicsScene * scene;
    int startx;
    int starty;
};

#endif // LEDITWIDGET_H
