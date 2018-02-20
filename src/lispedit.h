#ifndef LISPEDIT_H
#define LISPEDIT_H

#include <QObject>
#include <QCompleter>
#include <QStringListModel>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QStringList>
//#include <srchiliteqt/Qt4SyntaxHighlighter.h>

class LispEdit : public QPlainTextEdit
{
public:
    LispEdit(QWidget *parent);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    bool completedAndSelected;
    QCompleter * completer;
    QStringListModel * model;

    void createWidgets();
    void createConnections();
    void performCompletion(const QString &completionPrefix);
    void populateModel(const QString &completionPrefix);
    void insertCompletion(const QString &completion,bool singleWord);
    bool handledCompletedAndSelected(QKeyEvent *event);
protected slots:
    void highlightCurrentLine();
    void performCompletion();
};

#endif // LISPEDIT_H
