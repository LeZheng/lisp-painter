#include "mainwindow.h"
#include "lstartupwidget.h"
#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include "lispsymbolfactory.h"
#include <QDebug>

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
    qDebug() << "start...";
    int seed = QDateTime::currentDateTime().toTime_t();
    srand(seed);
    LispSymbolFactory * factory = LispSymbolFactory::getInstance();
    LStartupWidget s(3);
    s.connect(factory,&LispSymbolFactory::inited,&s,&LStartupWidget::initStateChange);
    s.show();
    s.move((QApplication::desktop()->width() - s.width())/2,
                   (QApplication::desktop()->height() - s.height())/2);

    MainWindow w;
    w.connect(&s,&LStartupWidget::finished,&w,&MainWindow::init);
    mainWindow = &w;
    w.setWindowTitle("lisp painter");
    w.setWindowIcon(QIcon(":/star"));
//    qInstallMessageHandler(messageOutput);
    factory->init();
    return a.exec();
}
