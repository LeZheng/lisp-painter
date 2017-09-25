#ifndef LISPSYNTAXHIGHLIGHTER_H
#define LISPSYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QProcess>

#define BASE_KEY_WORD "[(][a-zA-Z\-]*[ ]*"
#define KEY_WORD_ARGU ":[a-zA-Z\-]*[ $)]"
#define STRING_WORD "\".*\""
#define ARGU_TYPE_WORD "[ ]&[a-zA-Z]*"

class LispSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit LispSyntaxHighlighter(QObject * parent = 0);
    explicit LispSyntaxHighlighter(QTextDocument *parent = 0);
    ~LispSyntaxHighlighter();

protected:
    void highlightBlock(const QString &text);
private:
    QHash<QString,QString> symbolMap;
    QProcess * p;
};

#endif // LISPSYNTAXHIGHLIGHTER_H
