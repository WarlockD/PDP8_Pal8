#ifndef PAL8_HIGHLIGHTER_H
#define PAL8_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "qpal8.h"


class Pal8_SymbolDefine :  public QTextBlockUserData {
    Symbol* symbol;
  public:
    Pal8_SymbolDefine(Symbol* symbol, int line) : symbol(symbol) { symbol->line = line; }
    QString note;
  //  QString _extra_data;
    ~Pal8_SymbolDefine() {  if(symbol) { symbol->line = 0; symbol = nullptr; } }
};
enum class Pal8_LineInfo : int { Empty = 0, Code, EndCode };


class Pal8_Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Pal8_Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    QPal8_Parser _parser;
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};


#endif // PAL8_HIGHLIGHTER_H
