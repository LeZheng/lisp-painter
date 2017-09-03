#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

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
    MainWindow w;
    w.show();
    mainWindow = &w;
//    qInstallMessageHandler(messageOutput);
    return a.exec();
}
