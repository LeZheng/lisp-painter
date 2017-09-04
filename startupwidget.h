#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include <QWidget>

namespace Ui {
class StartupWidget;
}

class StartupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartupWidget(QWidget *parent = 0);
    ~StartupWidget();

private:
    Ui::StartupWidget *ui;
};

#endif // STARTUPWIDGET_H
