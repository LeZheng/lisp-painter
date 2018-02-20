#include "lispedit.h"

LispEdit::LispEdit(QWidget *parent)
    :QPlainTextEdit(parent),completedAndSelected(false)
{
    createWidgets();
    createConnections();
    highlightCurrentLine();
}

void LispEdit::createWidgets()
{
//    srchiliteqt::Qt4SyntaxHighlighter *highlighter =
//     new srchiliteqt::Qt4SyntaxHighlighter(document());
//    highlighter->init("lisp.lang");

    model = new QStringListModel(this);
    completer = new QCompleter(this);
    completer->setWidget(this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModel(model);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(true);
}

void LispEdit::createConnections()
{
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(highlightCurrentLine()));
    connect(completer,SIGNAL(activated(const QString&)),this,SLOT(insertPlainText(const QString&)));
    new QShortcut(QKeySequence(tr("Ctrl+M","Complete")),this,SLOT(performCompletion()));
}

void LispEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    QBrush highligherColor = palette().alternateBase();
    selection.format.setBackground(highligherColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection,true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

void LispEdit::performCompletion(){
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    const QString completionPrefix = cursor.selectedText();
    if(!completionPrefix.isEmpty() && completionPrefix.at(completionPrefix.length() - 1).isLetter())
    {
        performCompletion(completionPrefix);
    }
}

void LispEdit::performCompletion(const QString &completionPrefix)
{
    populateModel(completionPrefix);
    if(completionPrefix != completer->completionPrefix())
    {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0,0));
    }
    if(completer->completionCount() == 1)
    {
        insertCompletion(completer->currentCompletion(),true);
    }
    else
    {
        QRect rect = cursorRect();
        rect.setWidth(completer->popup()->sizeHintForColumn(0)
                      + completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(rect);
    }
}

bool caseInsensitiveLessThan(const QString &a,const QString &b)
{
    return a.compare(b,Qt::CaseInsensitive) < 0;
}

void LispEdit::populateModel(const QString &completionPrefix)
{
    //TODO need rewrite
    QStringList strings = toPlainText().split(QRegExp("\\W+"));
    strings.removeAll(completionPrefix);
    strings.removeDuplicates();
    qSort(strings.begin(),strings.end(),caseInsensitiveLessThan);
    model->setStringList(strings);
}

void LispEdit::insertCompletion(const QString &completion, bool singleWord)
{
    QTextCursor cursor = textCursor();
    int numberOfCharsToComplete = completion.length() - completer->completionPrefix().length();
    int insertionPosition = cursor.position();
    cursor.insertText(completion.right(numberOfCharsToComplete));
    if(singleWord){
        cursor.setPosition(insertionPosition);
        cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
        completedAndSelected = true;
    }
    setTextCursor(cursor);
}

void LispEdit::mousePressEvent(QMouseEvent *event)
{
    if(completedAndSelected)
    {
        completedAndSelected = false;
        QTextCursor cursor = textCursor();
        cursor.removeSelectedText();
        setTextCursor(cursor);
    }
    QPlainTextEdit::mousePressEvent(event);
}

void LispEdit::keyPressEvent(QKeyEvent *event)
{
    if(completedAndSelected && handledCompletedAndSelected(event))
        return;
    completedAndSelected = false;
    if(completer->popup()->isVisible())
    {
        switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape: event->ignore();return;
        default: completer->popup()->hide();break;
        }
    }
    QPlainTextEdit::keyPressEvent(event);
}

bool LispEdit::handledCompletedAndSelected(QKeyEvent *event)
{
    completedAndSelected = false;
    QTextCursor cursor = textCursor();
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return: cursor.clearSelection();break;
    case Qt::Key_Escape: cursor.removeSelectedText();break;
    default: return false;
    }

    setTextCursor(cursor);
    event->accept();
    return true;
}
