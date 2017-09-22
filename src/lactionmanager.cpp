#include "lactionmanager.h"

LActionManager * LActionManager::manager = NULL;

LActionManager * LActionManager::getInstance()
{
    if(manager == NULL)
    {
        manager = new LActionManager();
    }
    return manager;
}

LActionManager::LActionManager(QObject *parent) : QObject(parent)
{
    QAction * openAction = new QAction(QIcon(":/text-open"),tr("open"),this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip("open a file");//TODO
    actionMap["open"] = openAction;
    QAction * saveAction = new QAction(QIcon(":/save-only"),tr("save"),this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip("save all");//TODO
    actionMap["save"] = saveAction;
    QAction * createAction = new QAction(QIcon(":/text-new"),tr("new"),this);
    createAction->setShortcut(tr("Ctrl+N"));
    createAction->setStatusTip("new a file");//TODO
    actionMap["new"] = createAction;
    QAction * loadAction = new QAction(QIcon(":/load-only"),tr("load"),this);
    loadAction->setShortcut(tr("Ctrl+R"));
    loadAction->setStatusTip("load file by clisp");//TODO
    actionMap["load"] = loadAction;
    QAction * remindAction =  new QAction(QIcon(":/text-remind"),tr("remind"),this);
    remindAction->setShortcut(tr("Ctrl+1"));
    actionMap["remind"] = remindAction;
    QAction * drawRectAction = new QAction(QIcon(":/rect-scale"),tr("rect select"),this);
    drawRectAction->setShortcut(tr("Ctrl+Alt+a"));
    drawRectAction->setStatusTip("choose text with rect");//TODO
    actionMap["select"] = drawRectAction;
}

QAction * LActionManager::getAction(QString name)
{
    if(actionMap.contains(name))
        return actionMap[name];
    else
        return new QAction(name);
}

bool LActionManager::putAction(QString name,QAction * action)
{
    if(actionMap.contains(name))
    {
        return false;
    }
    else
    {
        action->setParent(this);
        actionMap[name] = action;
        return true;
    }
}

QAction * LActionManager::addAction(QString name,QString icon,QString shortcut,QString status)
{
    QAction * action = new QAction(QIcon(icon),name,this);
    action->setShortcut(shortcut);
    action->setStatusTip(status);
    actionMap[name] = action;
}
