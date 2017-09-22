#ifndef LACTIONMANAGER_H
#define LACTIONMANAGER_H

#include <QObject>
#include <QAction>
#include <QIcon>

class LActionManager : public QObject
{
    Q_OBJECT
public:
    static LActionManager * manager;
    static LActionManager * getInstance();
    QAction * getAction(QString name);
    bool putAction(QString name,QAction * action);
    QAction * addAction(QString name,QString icon,QString shortcut,QString status);
private:
    explicit LActionManager(QObject *parent = 0);
    QHash<QString,QAction *> actionMap;
signals:

public slots:
};

#endif // LACTIONMANAGER_H
