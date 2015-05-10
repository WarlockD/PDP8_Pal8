#ifndef QPAL8_H
#define QPAL8_H

#include <QObject>
#include <QString>
#include <QHash>
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
    QString _text;
    Kind _kind;
    int _pos;
public:
    // rule of 3
    Token(const Token& t) : _kind(t._kind),_pos(t._pos),_text(t._text) {}
    Token& operator=(const Token& r) {
        _text = r._text; _kind = r._kind; _pos = r._pos;
        return *this;
    }
    ~Token() {}

    Token() : _kind(Kind::Invalid),_pos(-1),_text("") {}
    Token(Kind kind, int pos);
    Token(Kind kind, const QString& text, int pos) : _kind(kind), _pos(pos),_text(text) {}
    // accessors
    inline QString text() const { return _text; }
    inline int len() const { return _text.length(); }
    inline int pos() const { return _pos; }
    inline  Kind kind() const { return _kind; }
};
inline bool operator==(const Token& l, const Token& r) { return l.kind() == r.kind(); }
inline bool operator!=(const Token& l, const Token& r) { return l.kind() != r.kind(); }
inline bool operator==(const Token& l, const Kind& r) { return l.kind() == r; }
inline bool operator!=(const Token& l, const Kind& r) { return l.kind() != r; }
inline bool operator==(const Token& l, const QString& r) { return l.text() == r; }
inline bool operator!=(const Token& l, const QString& r) { return l.text() != r; }


typedef QList<Token> Tokens;
typedef QList<Token>::iterator TokenIt;


class QPal8_Parser : public QObject
{
    Q_OBJECT
    const QString _src;
    Tokens _tokens;
  //  QHash<QString,Symbol*> _symtab;
public:
    explicit QPal8_Parser( QObject *parent = 0);
    static Tokens parseLine(const QString& src, bool ignoreComments=true);
   // const Token nextToken();
signals:

public slots:
};

#endif // QPAL8_H
