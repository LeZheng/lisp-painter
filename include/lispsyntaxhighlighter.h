#ifndef LISPSYNTAXHIGHLIGHTER_H
#define LISPSYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

#define BASE_KEY_WORD "[(][a-zA-Z\-]*\ "
#define KEY_WORD_ARGU ":[a-zA-Z\-]*[ \$)]"

class LispSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit LispSyntaxHighlighter(QObject * parent = 0);
    explicit LispSyntaxHighlighter(QTextDocument *parent = 0);
    ~LispSyntaxHighlighter();

protected:
    void highlightBlock(const QString &text);
};

#endif // LISPSYNTAXHIGHLIGHTER_H
