#include "lconsolewidget.h"
#include "ui_lconsolewidget.h"
#include <QDebug>

LConsoleWidget::LConsoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LConsoleWidget)
{
    ui->setupUi(this);
    this->process = new QProcess(this);
    connect(this->process,&QProcess::readyReadStandardOutput,this,&LConsoleWidget::readReady);
    connect(this->process,&QProcess::readyReadStandardError,this,&LConsoleWidget::readReady);

    connect(this->process,static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        done(exitCode);
        emit done(exitCode,result);
    });

    this->ui->consoleEdit->setReadOnly(true);
}

LConsoleWidget::~LConsoleWidget()
{
    delete ui;
}

void LConsoleWidget::processCmd(QString cmd)
{
    this->ui->consoleEdit->appendPlainText(">" + cmd);
    if(this->process->isTransactionStarted())
    {
        this->process->terminate();
    }
    result = "";
    this->process->start(cmd);
}

void LConsoleWidget::readReady()
{
    QString str = QString(this->process->readAll());
    this->ui->consoleEdit->appendPlainText(str);
    result = result + str;
}

void LConsoleWidget::done(int status)
{
    if(status != 0)
    {
        qWarning() << "process not success:" << status;
    }
}
