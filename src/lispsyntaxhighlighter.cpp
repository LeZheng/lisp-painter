#include "lispsyntaxhighlighter.h"
#include "lispsymbolfactory.h"
#include <QDebug>

LispSyntaxHighlighter::LispSyntaxHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{

}

LispSyntaxHighlighter::~LispSyntaxHighlighter()
{

}

void LispSyntaxHighlighter::highlightBlock(const QString &text)
{
    LispSymbolFactory * factory = LispSymbolFactory::getInstance();
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = BASE_KEY_WORD;//TODO
    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        LispSymbol * sym = factory->getSymbol(text.mid(index + 1,length - 1));
        if(sym != NULL)
        {
            setFormat(index + 1, length - 1, myClassFormat);
            delete sym;
        }
        index = text.indexOf(expression, index + length);
    }

    QListIterator<LHighlighterStrategy *> iterator(strategys);
    while (iterator.hasNext())
    {
        LHighlighterStrategy * strategy = iterator.next();
        int index = 0,length = 0;
        strategy->match(text,&index,&length);
        while(index >= 0){
            setFormat(index,length,strategy->getFormat());
            strategy->match(text,&index,&length,index + length);
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

LHighlighterStrategy::~LHighlighterStrategy()
{

}

QTextCharFormat LHighlighterStrategy::getFormat()
{
    return format;
}

LRegExpHighlighterStrategy::LRegExpHighlighterStrategy(QString r, LispSyntaxHighlighter *parent):
    LHighlighterStrategy(parent),regexp(r)
{

}

void LRegExpHighlighterStrategy::setFont(const QFont & f)
{
    format.setFont(f);
}

void LRegExpHighlighterStrategy::setForeground(const QBrush & b)
{
    format.setForeground(b);
}

void LRegExpHighlighterStrategy::setBackground(const QBrush & b)
{
    format.setBackground(b);
}

void LRegExpHighlighterStrategy::match(const QString & text,int * rIndex,int * rLength,int from)
{
    QRegExp argu_expression(regexp);
    *rIndex = text.indexOf(argu_expression, from);
    *rLength = argu_expression.matchedLength();
}
