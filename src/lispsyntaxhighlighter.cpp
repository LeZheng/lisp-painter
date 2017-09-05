#include "lispsyntaxhighlighter.h"
#include <QDebug>

LispSyntaxHighlighter::LispSyntaxHighlighter(QObject * parent):QSyntaxHighlighter(parent)
{

}

LispSyntaxHighlighter::LispSyntaxHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{
    p = new QProcess(this);QString::fromLatin1("");
    connect(p,static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int code,QProcess::ExitStatus exitStatus)
    {
        QByteArray qOutput = p->readAllStandardOutput();
        QList<QByteArray> list = qOutput.split('\n');
        QList<QByteArray>::iterator itor = list.begin();

        for ( ; itor != list.end(); itor++)
        {
            QByteArray strline = *itor;
            QString line = strline.simplified();
            int i = line.indexOf(" ");
            if(i == -1)
                continue;
            QString symbol = line.left(i);
            QString type = line.mid(i + 1);
            symbolMap[symbol] = type;
        }
    });
    p->start("clisp -i ~/init-symbol.lisp");//TODO
}

LispSyntaxHighlighter::~LispSyntaxHighlighter()
{

}

void LispSyntaxHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
    QString pattern = BASE_KEY_WORD;//TODO
    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index + 1, length - 1, myClassFormat);
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
