#include "startupwidget.h"
#include "ui_startupwidget.h"

StartupWidget::StartupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartupWidget)
{
    ui->setupUi(this);
}

StartupWidget::~StartupWidget()
{
    delete ui;
}
