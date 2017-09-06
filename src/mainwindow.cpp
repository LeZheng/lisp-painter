#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    leftDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea,leftDock);


    connect(fw,&LFileWidget::itemSelected,this->editWidget,&LEditWidget::open);

    consoleWidget = new LConsoleWidget(this);
    QDockWidget * bottomDock = new QDockWidget(this);
    bottomDock->setWidget(consoleWidget);
    bottomDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea,bottomDock);

    baseToolBar = addToolBar("base");

    fileMenu = menuBar()->addMenu(tr("file"));
    openAction = baseToolBar->addAction(QIcon(":/text-open"),tr("open"));
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip("open a file");//TODO
    saveAction = baseToolBar->addAction(QIcon(":/save-only"),tr("save"));
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip("save all");//TODO
    createAction = baseToolBar->addAction(QIcon(":/text-new"),tr("new"));
    createAction->setShortcut(tr("Ctrl+N"));
    createAction->setStatusTip("new a file");//TODO
    runAction = baseToolBar->addAction(QIcon(":/load-only"),tr("load"));
    runAction->setShortcut(tr("Ctrl+R"));
    runAction->setStatusTip("load file by clisp");//TODO
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(createAction);
    fileMenu->addAction(runAction);
    connect(openAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getOpenFileName(this);
        editWidget->open(path);
    });
    connect(saveAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = this->ui->tabWidget->tabText(this->ui->tabWidget->currentIndex());
        editWidget->save(path);
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

    fontToolBar = addToolBar("Font");
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addWidget(colorBtn);
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
