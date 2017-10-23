#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumHeight(300);
    this->setMinimumWidth(400);
    this->statusBar()->setSizeGripEnabled(false);
    this->editWidget = new LEditWidget(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    QApplication::quit();
}

void MainWindow::initBaseToolBar()
{
    LActionManager * manager = LActionManager::getInstance();
    QAction *openAction = manager->addAction(tr("open"),":/text-open","Ctrl+O","open a file");
    QAction *saveAction = manager->addAction(tr("save"),":/save-only","Ctrl+S","save all");
    QAction *createAction = manager->addAction(tr("new"),":/text-new","Ctrl+N","new a file");
    QAction *runAction = manager->addAction(tr("load"),":/load-only","Ctrl+R","load file by clisp");
    QAction *remindAction = manager->addAction(tr("remind"),":/text-remind","Ctrl+1","symbol remind");
    QAction *drawRectAction = manager->addAction(tr("rect select"),":/rect-scale","Ctrl+Alt+a","choose text with rect");
    toolWidget->addAction("base",openAction);
    toolWidget->addAction("base",saveAction);
    toolWidget->addAction("base",createAction);
    toolWidget->addAction("base",runAction);
    toolWidget->addAction("base",remindAction);
    toolWidget->addAction("base",drawRectAction);
    connect(openAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getOpenFileName(this);
        this->editWidget->open(path);
    });
    connect(saveAction,&QAction::triggered,
            [=](bool checked)
    {
        this->editWidget->save("");
    });
    connect(createAction,&QAction::triggered,
            [=](bool checked)
    {
        QString path = QFileDialog::getSaveFileName(this);
        this->editWidget->create(path);
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

    QMenuBar * bar = menuBar();
    QMenu * fileMenu = bar->addMenu(tr("file"));
    fileMenu->addAction(openAction->text(),openAction,&QAction::trigger,openAction->shortcut())->setStatusTip(openAction->statusTip());
    fileMenu->addAction(saveAction->text(),saveAction,&QAction::trigger,saveAction->shortcut())->setStatusTip(saveAction->statusTip());
    fileMenu->addAction(createAction->text(),createAction,&QAction::trigger,createAction->shortcut())->setStatusTip(createAction->statusTip());
    QMenu * buildMenu = bar->addMenu(tr("build"));
    buildMenu->addAction(runAction->text(),runAction,&QAction::trigger,runAction->shortcut())->setStatusTip(runAction->statusTip());
}

void MainWindow::initFontToolBar()
{
    QToolBar * fontToolBar = addToolBar("Font");
    QLabel * fontLabel1 = new QLabel(tr("font"),fontToolBar);
    QFontComboBox * fontComboBox = new QFontComboBox(fontToolBar);
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    QLabel * fontLabel2 = new QLabel(tr("size"),fontToolBar);
    QComboBox * sizeComboBox = new QComboBox(fontToolBar);
    QFontDatabase db;
    foreach (int size, db.standardSizes())
    {
        sizeComboBox->addItem(QString::number(size));
    }
    QToolButton * boldBtn = new QToolButton(fontToolBar);
    boldBtn->setText("bold");
    boldBtn->setCheckable(true);
    QToolButton * italicBtn = new QToolButton(fontToolBar);
    italicBtn->setText("italic");
    italicBtn->setCheckable(true);
    QToolButton * underlineBtn = new QToolButton(fontToolBar);
    underlineBtn->setText("underline");
    underlineBtn->setCheckable(true);
    QToolButton * colorBtn = new QToolButton(fontToolBar);
    colorBtn->setText("color");
    colorBtn->setCheckable(true);
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addWidget(colorBtn);
    toolWidget->addPage("Font",fontToolBar);

    connect(fontComboBox,static_cast<void(QComboBox::*)(const QString &)>(&QFontComboBox::activated),editWidget,&LEditWidget::changeFontType);
    connect(sizeComboBox,static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),editWidget,&LEditWidget::changeFontSize);
    connect(boldBtn,&QToolButton::clicked,editWidget,&LEditWidget::changeToBold);
    connect(italicBtn,&QToolButton::clicked,editWidget,&LEditWidget::changeToItalic);
    connect(underlineBtn,&QToolButton::clicked,editWidget,&LEditWidget::changeToUnderline);
    connect(colorBtn,&QToolButton::clicked,editWidget,&LEditWidget::changeColor);
    connect(editWidget,&LEditWidget::currentCharFormatChanged,
            [=](const QTextCharFormat &fmt)
    {
        fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
        sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
        boldBtn->setChecked(fmt.font().bold());
        italicBtn->setChecked(fmt.font().italic());
        underlineBtn->setChecked(fmt.font().underline());
    });
}

void MainWindow::initFloatDock()
{
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    fdw = new LFloatDockWidget(desktopRect.left(),100,200,desktopRect.height() - 100 * 2);
    floatWidgets.append(fdw);
    QVBoxLayout * layout = new QVBoxLayout(fdw);
    layout->setMargin(6);
    LFileWidget * fw = new LFileWidget(fdw);
    layout->addWidget(fw);
    fdw->show();
    connect(fw,&LFileWidget::itemSelected,
            [=](QString path)
    {
        this->editWidget->open(path);
    });
    connect(this,&MainWindow::destroyed,fdw,&LFloatDockWidget::deleteLater);

    tdw = new LFloatDockWidget(300,0,desktopRect.width() - 300 * 2,90);
    floatWidgets.append(tdw);
    QVBoxLayout * tlayout = new QVBoxLayout(tdw);
    tlayout->setMargin(6);
    toolWidget = new LToolsWidget(tdw);
    tlayout->addWidget(toolWidget);
    tdw->show();

    cdw = new LFloatDockWidget(300,desktopRect.bottom() - 150 + 1,desktopRect.width() - 300 * 2,150);
    floatWidgets.append(cdw);
    consoleWidget = new LConsoleWidget(cdw);
    QVBoxLayout * clayout = new QVBoxLayout(cdw);
    clayout->setMargin(6);
    clayout->addWidget(consoleWidget);
    cdw->show();
}

void MainWindow::initSplitWidget()
{
    LSplitCopyWidget * scw = new LSplitCopyWidget(editWidget,this);
    LActionManager * manager = LActionManager::getInstance();
    QAction * splitVAction = manager->addAction(tr("split vertical"),":/split-v","Ctrl+Shift+O","split vertically");
    QAction * splitHAction = manager->addAction(tr("split horizontal"),":/split-h","Ctrl+Shift+E","split horizontally");
    QAction * closeSAction = manager->addAction(tr("close split widget"),":/close-split","Ctrl+Shift+W","close current split widget");
    toolWidget->addAction("base",splitVAction);
    toolWidget->addAction("base",splitHAction);
    toolWidget->addAction("base",closeSAction);
    QMenu * widgetMenu = menuBar()->addMenu(tr("widget"));
    widgetMenu->addAction(splitVAction->text(),splitVAction,&QAction::trigger,splitVAction->shortcut())->setStatusTip(splitVAction->statusTip());
    widgetMenu->addAction(splitHAction->text(),splitHAction,&QAction::trigger,splitHAction->shortcut())->setStatusTip(splitHAction->statusTip());
    widgetMenu->addAction(closeSAction->text(),closeSAction,&QAction::trigger,closeSAction->shortcut())->setStatusTip(closeSAction->statusTip());
    connect(splitHAction,&QAction::triggered,scw,&LSplitCopyWidget::horizontalSplit);
    connect(splitVAction,&QAction::triggered,scw,&LSplitCopyWidget::verticalSplit);
    connect(closeSAction,&QAction::triggered,scw,&LSplitCopyWidget::closeWidget);
    connect(scw,&LSplitCopyWidget::widgetActive,
            [=](LSplitCopyWidget * w)
    {
        LCloneableWidget * widget = w->getCurrentWidget();
        if(widget->inherits("LEditWidget"))
        {
            this->editWidget = qobject_cast<LEditWidget *>(widget);
        }
    });
    setCentralWidget(scw);
}

void MainWindow::init()
{
    initFloatDock();
    initBaseToolBar();
    initFontToolBar();
    initSplitWidget();
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    move((desktopRect.width() - width())/2,(desktopRect.height() - height())/2);
    this->show();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QRect mRect = frameGeometry();
    QListIterator<LFloatDockWidget *> iterator(floatWidgets);
    while(iterator.hasNext())
    {
        LFloatDockWidget * dw = iterator.next();
        QRect dwRect = dw->originRect;
        int minX = mRect.left() > dwRect.left() ? dwRect.left() : mRect.left();
        int maxX = mRect.right() > dwRect.right() ? mRect.right() : dwRect.right();
        int minY = mRect.top() > dwRect.top() ? dwRect.top() : mRect.top();
        int maxY = mRect.bottom() > dwRect.bottom() ? mRect.bottom() : dwRect.bottom();
        if(maxX - minX < mRect.width() + dwRect.width() && maxY - minY < mRect.height() + dwRect.height())
        {
            dw->setOverlap(true);
            dw->appearOrDisappear(false);
        }
        else
        {
            dw->setOverlap(false);
            dw->appearOrDisappear(true);
        }
    }
}
