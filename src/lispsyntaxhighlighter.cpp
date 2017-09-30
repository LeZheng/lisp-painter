#include "lispsyntaxhighlighter.h"
#include "lispsymbolfactory.h"
#include <QDebug>

LispSyntaxHighlighter::LispSyntaxHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent){ }

LispSyntaxHighlighter::~LispSyntaxHighlighter(){ }

void LispSyntaxHighlighter::highlightBlock(const QString &text)
{
    QListIterator<LHighlighterStrategy *> iterator(strategys);
    while (iterator.hasNext())
    {
        LHighlighterStrategy * strategy = iterator.next();
        int index = 0,length = 0;
        strategy->match(text,&index,&length);
        while(index >= 0){
            setFormat(index,length,strategy->getFormat());
            strategy->match(text,&index,&length,length == 0?index + 1:index + length);
        }
    }
}

void LispSyntaxHighlighter::addStrategy(LHighlighterStrategy * strategy)
{
    strategys.append(strategy);
}

LHighlighterStrategy::LHighlighterStrategy(LispSyntaxHighlighter * parent):
    QObject(parent)
{
    parent->addStrategy(this);
}

LHighlighterStrategy::~LHighlighterStrategy(){ }

void LHighlighterStrategy::setFont(const QFont & f)
{
    format.setFont(f);
}

void LHighlighterStrategy::setForeground(const QBrush & b)
{
    format.setForeground(b);
}

void LHighlighterStrategy::setBackground(const QBrush & b)
{
    format.setBackground(b);
}

QTextCharFormat LHighlighterStrategy::getFormat()
{
    return format;
}

LRegExpHighlighterStrategy::LRegExpHighlighterStrategy(QString r, LispSyntaxHighlighter *parent):
    LHighlighterStrategy(parent),regexp(r){ }

void LRegExpHighlighterStrategy::match(const QString & text,int * rIndex,int * rLength,int from)
{
    QRegExp argu_expression(regexp);
    *rIndex = text.indexOf(argu_expression, from);
    *rLength = argu_expression.matchedLength();
}

LSymbolHighlighterStrategy::LSymbolHighlighterStrategy(LispSyntaxHighlighter * parent)
    : LHighlighterStrategy(parent){ }

void LSymbolHighlighterStrategy::match(const QString & text,int * rIndex,int * rLength,int from)
{
    LispSymbolFactory * factory = LispSymbolFactory::getInstance();
    QString pattern = BASE_KEY_WORD;
    QRegExp expression(pattern);
    *rIndex = text.indexOf(expression,from);
    if (*rIndex >= 0) {
        *rLength = expression.matchedLength();
        LispSymbol * sym = factory->getSymbol(text.mid(*rIndex + 1,*rLength - 1));
        if(sym != NULL)
        {
            *rIndex = *rIndex + 1;
            *rLength = *rLength - 1;
            delete sym;
        }
        else
        {
            *rLength = 0;
        }
    }
}
