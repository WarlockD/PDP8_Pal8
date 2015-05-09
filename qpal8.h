#ifndef QPAL8_H
#define QPAL8_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QVector>
#include <iostream>
class Symbol  {

public:
    Symbol(const QString& name) :  name(name),value(0),mri(false),fixed(false),line(0),label(false) {}
    QString name;
    int value;
    bool mri;
    bool fixed;
    bool label;
    int line;
    inline bool defined() const { return line > 0; }
    bool operator==(const Symbol& o) const { return name == o.name; }
};

//uint qHash(const Symbol&ptr,int seed=0);
inline uint qHash(const Symbol*ptr) { return qHash(ptr->name); }


enum class Kind { Invalid, Literal, Ident, Assign, Label, Add, Minus, ImplicitOr, LParm, RParm,Comment,Error };
class Token  {
    QStringRef _text;
    Kind _kind;
public:
    // rule of 3
    Token(const Token& t) : _kind(t._kind),_text(t._text) {}
    Token& operator=(const Token& r) {
        _text = r._text; _kind = r._kind;
        return *this;
    }
    ~Token() {}

    Token() : _kind(Kind::Invalid), _text() {}
    Token(Kind kind, QStringRef text) : _kind(kind), _text(text) {}
    // accessors
    inline QStringRef text() const { return _text; }
    inline int len() const { return _text.length(); }
    inline int pos() const { return _text.position(); }
    inline  Kind kind() const { return _kind; }
    QString debug() const;
};


inline bool operator==(const Token& l, const Token& r) { return l.kind() == r.kind(); }
inline bool operator!=(const Token& l, const Token& r) { return l.kind() != r.kind(); }
inline bool operator==(const Token& l, const Kind& r) { return l.kind() == r; }
inline bool operator!=(const Token& l, const Kind& r) { return l.kind() != r; }
inline bool operator==(const Token& l, const QString& r) { return l.text() == r; }
inline bool operator!=(const Token& l, const QString& r) { return l.text() != r; }


typedef QVector<Token> Tokens;
typedef QVector<Token>::iterator TokenIt;

class TokenLine {
    QString _line;
    int _lineno;
    Tokens _tokens;
public:
    TokenLine(const QString& str, int lineno);
    void setLine(const QString& str, int lineno);
    inline const Tokens& tokens() { return _tokens; }
};


class QPal8_Parser : public QObject
{
    Q_OBJECT
    const QString _src;
    Tokens _tokens;
    QVector<TokenLine> _lines;

public:
     QHash<QString,Symbol*> symtab;
     QHash<int,QString> asm_line;
    explicit QPal8_Parser( QObject *parent = 0);
    static Tokens parseLine(const QString& src, bool ignoreComments=true);

    Tokens assemble(const QString& src);
   // const Token nextToken();
signals:

public slots:
};

#endif // QPAL8_H
