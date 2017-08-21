#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class LConsoleWidget;
}

class LConsoleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LConsoleWidget(QWidget *parent = 0);
    ~LConsoleWidget();
signals:
    void done(int status,QString result);

public slots:
    void processCmd(QString cmd);

private slots:
    void readReady();
    void done(int status);

private:
    Ui::LConsoleWidget *ui;
    QProcess * process;
    QString result;
};

#endif // CONSOLEWIDGET_H
