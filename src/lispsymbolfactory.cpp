#include "lispsymbolfactory.h"
#include <QDebug>

LispSymbol::LispSymbol(QString name,
                       QString type,
                       QString s,
                       QString ms,
                       QString av,
                       QString desc,
                       QObject *parent) :
    QObject(parent),sname(name),stype(type),ssyntax(s),smethodSignatures(ms),sargumentsAndValues(av),sdescription(desc)
{

}

QString LispSymbol::name()
{
    return sname;
}

QString LispSymbol::type()
{
    return stype;
}

QString LispSymbol::syntax()
{
    return ssyntax;
}

QString LispSymbol::methodSignatures()
{
    return smethodSignatures;
}

QString LispSymbol::argumentsAndValues()
{
    return sargumentsAndValues;
}

QString LispSymbol::description()
{
    return sdescription;
}

LispSymbolFactory::LispSymbolFactory(QObject *parent) : QObject(parent)
{
    p = new QProcess(this);
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
            int start = line.indexOf("| ");
            int i = line.indexOf(" ",start == -1 ? 0 : start);
            if(i == -1)
                continue;
            QString symbol = line.left(i);
            QString type = line.mid(i + 1);
            symbolMap[symbol] = type;
        }
        if(code == 0)
            emit inited();
        else
            qWarning() << code << ":init failed!!";
    });
    p->start("clisp -i ~/init-symbol.lisp");//TODO
}

LispSymbolFactory * LispSymbolFactory::instance = NULL;

LispSymbolFactory * LispSymbolFactory::getInstance()
{
    if(LispSymbolFactory::instance == NULL)
    {
        instance = new LispSymbolFactory;
    }
    return instance;
}

LispSymbol * LispSymbolFactory::getSymbol(QString name)
{
    name = name.trimmed();
    if(!symbolMap.contains(name))
    {
        name = name.toUpper();
    }
    if(symbolMap.contains(name))
    {
        LispSymbol * sym = new LispSymbol(name,symbolMap[name]);
        return sym;
    }
    else
    {
        return NULL;
    }
}
