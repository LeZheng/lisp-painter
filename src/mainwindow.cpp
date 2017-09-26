#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lactionmanager.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumHeight(600);
    this->setMinimumWidth(800);

    this->editWidget = new LEditWidget(this);
    this->editWidget->setMinimumHeight(300);
    this->editWidget->setMinimumWidth(400);

    setCentralWidget(editWidget);

    LFileWidget * fw = new LFileWidget(this);
    QDockWidget * leftDock = new QDockWidget(this);
    leftDock->setWidget(fw);
    leftDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);


    connect(fw,&LFileWidget::itemSelected,this->editWidget,&LEditWidget::open);

    consoleWidget = new LConsoleWidget(this);
    QDockWidget * bottomDock = new QDockWidget(this);
    bottomDock->setWidget(consoleWidget);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea,bottomDock);

    baseToolBar = addToolBar("base");

    LActionManager * manager = LActionManager::getInstance();
    openAction = manager->addAction(tr("open"),":/text-open","Ctrl+O","open a file");
    saveAction = manager->addAction(tr("save"),":/save-only","Ctrl+S","save all");
    createAction = manager->addAction(tr("new"),":/text-new","Ctrl+N","new a file");
    runAction = manager->addAction(tr("load"),":/load-only","Ctrl+R","load file by clisp");
    remindAction = manager->addAction(tr("remind"),":/text-remind","Ctrl+1","symbol remind");
    drawRectAction = manager->addAction(tr("rect select"),":/rect-scale","Ctrl+Alt+a","choose text with rect");
    connect(openAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getOpenFileName(this);
        editWidget->open(path);
    });
    connect(saveAction,&QAction::triggered,
            [=](bool checked)
    {
        editWidget->save("");
    });
    connect(createAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getSaveFileName(this);
        editWidget->create(path);
    });
    connect(runAction,&QAction::triggered,
            [=](bool checked)
    {
        this->consoleWidget->processCmd("clisp " + path);
    });
    connect(this->editWidget,&LEditWidget::currentFileChanged,
            [=](QString path)
    {
        this->path = path;
    });
    connect(remindAction,&QAction::triggered,editWidget,&LEditWidget::selectCurrentWord);

    toolWidget = new LToolsWidget(this);
    QDockWidget * toolDock = new QDockWidget(this);
    toolDock->setWidget(toolWidget);
    toolDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::TopDockWidgetArea,toolDock);

    toolWidget->addAction("base",openAction);
    toolWidget->addAction("base",saveAction);
    toolWidget->addAction("base",createAction);
    toolWidget->addAction("base",runAction);
    toolWidget->addAction("base",remindAction);
    toolWidget->addAction("base",drawRectAction);

    initToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initToolBar()
{
    this->fontLabel1 = new QLabel(tr("font"));
    this->fontComboBox = new QFontComboBox();
    this->fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);

    this->fontLabel2 = new QLabel(tr("size"));
    this->sizeComboBox = new QComboBox();
    QFontDatabase db;
    foreach (int size, db.standardSizes())
    {
        this->sizeComboBox->addItem(QString::number(size));
    }

    this->boldBtn = new QToolButton();
    this->boldBtn->setText("bold");
    this->boldBtn->setCheckable(true);

    this->italicBtn = new QToolButton();
    this->italicBtn->setText("italic");
    this->italicBtn->setCheckable(true);

    this->underlineBtn = new QToolButton();
    this->underlineBtn->setText("underline");
    this->underlineBtn->setCheckable(true);

    this->colorBtn = new QToolButton();
    this->colorBtn->setText("color");
    this->colorBtn->setCheckable(true);

    connect(this->fontComboBox,static_cast<void(QComboBox::*)(const QString &)>(&QFontComboBox::activated),this,&MainWindow::showFontComboBox);
    connect(this->sizeComboBox,static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),this,&MainWindow::showSizeSpinBox);
    connect(this->boldBtn,&QToolButton::clicked,this,&MainWindow::showBoldBtn);
    connect(this->italicBtn,&QToolButton::clicked,this,&MainWindow::showItalicBtn);
    connect(this->underlineBtn,&QToolButton::clicked,this,&MainWindow::showUnderlineBtn);
    connect(this->colorBtn,&QToolButton::clicked,this,&MainWindow::showColorBtn);
    connect(this->editWidget,&LEditWidget::currentCharFormatChanged,this,&MainWindow::showCurrentFormatChanged);
    connect(this,&MainWindow::currentCharFormatChanged,this->editWidget,&LEditWidget::mergeFormat);

    fontToolBar = addToolBar("Font");//new QToolBar();
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addWidget(colorBtn);
//    toolWidget->addPage("font",fontToolBar);
}

void MainWindow::showFontComboBox(QString font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    emit currentCharFormatChanged(fmt);
}

void MainWindow::showSizeSpinBox(QString value)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(value.toFloat());
    emit currentCharFormatChanged(fmt);
}
void MainWindow::showBoldBtn()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(this->boldBtn->isChecked() ? QFont::Bold : QFont::Normal);
    emit currentCharFormatChanged(fmt);
}
void MainWindow::showItalicBtn()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(this->italicBtn->isChecked());
    emit currentCharFormatChanged(fmt);
}
void MainWindow::showUnderlineBtn()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(this->underlineBtn->isChecked());
    emit currentCharFormatChanged(fmt);
}
void MainWindow::showColorBtn()
{

    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        emit currentCharFormatChanged(fmt);
    }
    else
    {
        //TODO color is not valid
    }
}
void MainWindow::showCurrentFormatChanged(const QTextCharFormat &fmt)
{
    fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
    sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.font().italic());
    underlineBtn->setChecked(fmt.font().underline());
}
