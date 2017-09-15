#include "mainwindow.h"
#include "startupwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include "lispsymbolfactory.h"

QMainWindow * mainWindow;

static void messageOutput(QtMsgType type,const QMessageLogContext &context,const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtWarningMsg:
        QMessageBox::information(mainWindow,"warning",QString(localMsg.constData()));
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int seed = QDateTime::currentDateTime().toTime_t();
    srand(seed);
    LispSymbolFactory * factory = LispSymbolFactory::getInstance();
    StartupWidget s(1);
    s.connect(factory,&LispSymbolFactory::inited,&s,&StartupWidget::initStateChange);
    factory->init();
    s.show();
    s.move((QApplication::desktop()->width() - s.width())/2,
                   (QApplication::desktop()->height() - s.height())/2);
    MainWindow w;
    w.connect(&s,&StartupWidget::finished,&w,&MainWindow::show);
    mainWindow = &w;
//    qInstallMessageHandler(messageOutput);
    return a.exec();
}
