#include "lispsyntaxhighlighter.h"
#include "lispsymbolfactory.h"
#include <QDebug>

LispSyntaxHighlighter::LispSyntaxHighlighter(QObject * parent):QSyntaxHighlighter(parent)
{

}

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
//        LispSymbol * sym = factory->getSymbol(text.mid(index + 1,length - 2));
//        if(sym != NULL)
//        {
            setFormat(index + 1, length - 1, myClassFormat);
//            delete sym;
//        }
        index = text.indexOf(expression, index + length);
    }

    myClassFormat.setForeground(Qt::yellow);
    pattern = KEY_WORD_ARGU;//TODO
    QRegExp kargu_expression(pattern);
    index = text.indexOf(kargu_expression);
    while (index >= 0) {
        int length = kargu_expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(kargu_expression, index + length);
    }
}
