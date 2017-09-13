#ifndef LISPSYMBOLFACTORY_H
#define LISPSYMBOLFACTORY_H

#include <QObject>
#include <QProcess>
#include <QHash>

class LispSymbol : public QObject
{
    Q_OBJECT
public:
    explicit LispSymbol(QString name,
                        QString type,
                        QString syntax = "",
                        QString methodSignatures = "",
                        QString argumentsAndValues = "",
                        QString description = "",
                        QObject *parent = 0);
    QString name();
    QString type();
    QString syntax();
    QString methodSignatures();
    QString argumentsAndValues();
    QString description();
private:
    const QString sname;
    const QString stype;
    const QString ssyntax;
    const QString smethodSignatures;
    const QString sargumentsAndValues;
    const QString sdescription;
};

class LispSymbolFactory : public QObject
{
    Q_OBJECT
public:
    static LispSymbolFactory * getInstance();
    LispSymbol * getSymbol(QString name);
    void init();
private:
    explicit LispSymbolFactory(QObject *parent = 0);
    static LispSymbolFactory * instance;
    QHash<QString,QString> symbolMap;
    QProcess * p;
signals:
    void inited(QString info);
public slots:
};

#endif // LISPSYMBOLFACTORY_H
