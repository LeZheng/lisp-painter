#ifndef LTOOLSWIDGET_H
#define LTOOLSWIDGET_H

#include <QWidget>
#include <QToolBox>
#include <QToolBar>
#include <QToolButton>
#include <QGridLayout>

namespace Ui {
class LToolsWidget;
}

class LToolsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LToolsWidget(QWidget *parent = 0);
    ~LToolsWidget();
    QAction * addWidget(QString pageName,QWidget * w);
    bool addPage(QString pageName,QWidget * page);
    void addAction(QString pageName,QAction * action);
    QAction * addAction(QString pageName,QString action,QIcon icon,QString shortcut,QString status);
private:
    Ui::LToolsWidget *ui;
    QToolBox * toolBox;
    QHash<QString,QWidget *> pageMap;
};

#endif // LTOOLSWIDGET_H
