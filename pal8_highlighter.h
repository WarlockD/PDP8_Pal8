#ifndef PAL8_HIGHLIGHTER_H
#define PAL8_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Symbol : public QObject {
    Q_OBJECT
public:
    Symbol(QTextDocument *parent = 0) : QObject(parent) {}
    QString name;
    int value;
    bool label;
    int line_defined;
    bool operator==(const Symbol& o) { return name == o.name; }
};

class Pal8_SymbolDefine : public QObject, public QTextBlockUserData {
    Q_OBJECT
public:
    Pal8_SymbolDefine(QObject *parent = 0);
public:
    QPointer<Symbol> symbol;
    int line_number;
    int address;
    int data;
    QString _extra_data;
    ~Pa8_LineData() { if(symbol) { symbol->line_defined = -1; symbol = nullptr; } }
};
enum class Pal8_LineInfo : int { Empty = 0, Code, EndCode };
uint qHash(const QPointer<MyObject>&ptr){ return qHash(ptr.name); }

class Pal8_Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Pal8_Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:

    QHash<QString,QPointer<Symbol>> symtab;

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
