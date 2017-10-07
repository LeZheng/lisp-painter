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

QList<LispSymbol *> LispSymbolFactory::getSymbols(QString name)
{
    QList<LispSymbol *> symbols;
    QProcess proc;
    proc.start(QString("/home/skyline/get-symbols.mem %1").arg(name));

    if (!proc.waitForFinished())
         qWarning() << "proc.waitForFinished error";

    QByteArray result = proc.readAll();
    QList<QByteArray> list = result.split('\n');
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
        LispSymbol * sym = new LispSymbol(symbol,type);
        symbols.append(sym);
    }
    return symbols;
}

void LispSymbolFactory::init()
{
    this->setAutoDelete(false);
    QThreadPool::globalInstance()->start(this);
}

void LispSymbolFactory::run()
{
    p = new QProcess();
    QString tmpPath = QDir::tempPath();
    QFile::copy(":/init-mem",QString("%1/init-symbol.lisp").arg(tmpPath));
    emit inited("download init lisp file successfully");

    p->start(QString("clisp -i %1/init-symbol.lisp").arg(tmpPath));
    p->waitForFinished();
    if(p->exitCode() == 0)
        emit inited("create get-symbols.mem file successfully");
    else
    {
        qWarning() << "create get-symbols.mem file failed";
        //TODO dialog and exit
    }

    p->start("/home/skyline/get-symbols.mem");
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
    if(p->exitCode() == 0)
    {
        emit inited("load symbol from clisp successfully");
    }
    else
    {
        qWarning() << p->exitCode() << ":init failed!!";
        emit inited("load symbol from clisp failed");
    }
}
