#include "lispsyntaxhighlighter.h"

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
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = "defun|let";//TODO

    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }
}
