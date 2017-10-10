#ifndef LFILEWIDGET_H
#define LFILEWIDGET_H

#include <QWidget>
#include <QDirModel>
#include <QStack>

namespace Ui {
class LFileWidget;
}

class LFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LFileWidget(QWidget *parent = 0);
    ~LFileWidget();
signals:
    void itemSelected(QString path);
private:
    Ui::LFileWidget *ui;
    QDirModel * model;
    QString workPath;
    QStack<QModelIndex> historyPathStack;
};

#endif // LFILEWIDGET_H
