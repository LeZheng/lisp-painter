#ifndef LEDITWIDGET_H
#define LEDITWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>

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
private:
    Ui::LEditWidget *ui;
    QMap<QString,QTextEdit *> edits;
};

#endif // LEDITWIDGET_H
