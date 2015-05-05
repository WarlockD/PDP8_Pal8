#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include "pal8_highlighter.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void assemblyAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    int assemblyAreaWidth();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateAssemblyAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateAssemblyAreaArea(const QRect &, int);
private:
    QWidget *lineNumberArea;
    QWidget *assemblyArea;
    Pal8_Highlighter *highlighter;
};

class LineNumberArea : public QWidget
{
    Q_OBJECT
private:
    CodeEditor *codeEditor;
public:
    explicit LineNumberArea(CodeEditor *editor): QWidget(editor) { codeEditor = editor; }
    QSize sizeHint() const Q_DECL_OVERRIDE { return QSize(codeEditor->lineNumberAreaWidth(), 0); }
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE { codeEditor->lineNumberAreaPaintEvent(event); }
};

class AssemblyArea : public QWidget
{
    Q_OBJECT
private:
    CodeEditor *codeEditor;
public:
    explicit AssemblyArea(CodeEditor *editor): QWidget(editor) { codeEditor = editor; }
    QSize sizeHint() const Q_DECL_OVERRIDE { return QSize(codeEditor->assemblyAreaWidth(), 0); }
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE { codeEditor->assemblyAreaPaintEvent(event); }
};
#endif // CODEEDITOR_H
