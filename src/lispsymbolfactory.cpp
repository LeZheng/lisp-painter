#include "lispsymbolfactory.h"

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
