#ifndef LISPSYNTAXHIGHLIGHTER_H
#define LISPSYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

#define BASE_KEY_WORD "[(][a-zA-Z\-]*[ ]*"
#define KEY_WORD_ARGU ":[a-zA-Z\-]*[ $)]"
#define STRING_WORD "[ ]\".*\""
#define ARGU_TYPE_WORD "[ ]&[a-zA-Z]*"

class LHighlighterStrategy;
class LispSyntaxHighlighter;
class LRegExpHighlighterStrategy;

class LispSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit LispSyntaxHighlighter(QTextDocument *parent = 0);
    ~LispSyntaxHighlighter();
    void addStrategy(LHighlighterStrategy * strategy);
protected:
    void highlightBlock(const QString &text);
private:
    QList<LHighlighterStrategy *> strategys;
};

class LHighlighterStrategy : public QObject
{
    Q_OBJECT
public:
    explicit LHighlighterStrategy(LispSyntaxHighlighter * parent = 0);
    ~LHighlighterStrategy();
    virtual void match(const QString & text,int * rIndex,int * rLength,int from = 0) = 0;
    QTextCharFormat getFormat();
    void setFont(const QFont &);
    void setForeground(const QBrush &);
    void setBackground(const QBrush &);
protected:
    QTextCharFormat format;
};

class LRegExpHighlighterStrategy : public LHighlighterStrategy
{
    Q_OBJECT
public:
    explicit LRegExpHighlighterStrategy(QString regexp,LispSyntaxHighlighter * parent = 0);
    virtual void match(const QString & text,int * rIndex,int * rLength,int from = 0);
private:
    const QString regexp;
};

class LSymbolHighlighterStrategy : public LHighlighterStrategy
{
    Q_OBJECT
public:
    explicit LSymbolHighlighterStrategy(LispSyntaxHighlighter * parent = 0);
    virtual void match(const QString & text,int * rIndex,int * rLength,int from = 0);
};

#endif // LISPSYNTAXHIGHLIGHTER_H
