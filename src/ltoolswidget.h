#ifndef LTOOLSWIDGET_H
#define LTOOLSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QToolButton>
#include <QGridLayout>
#include <QObjectCleanupHandler>

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
    QTabWidget * tabWidget;
    QHash<QString,QWidget *> pageMap;
    QObjectCleanupHandler cleaner;
};

#endif // LTOOLSWIDGET_H
