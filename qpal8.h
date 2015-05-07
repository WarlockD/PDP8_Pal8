#ifndef QPAL8_H
#define QPAL8_H

#include <QObject>

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

uint qHash(const Symbol&ptr);
uint qHash(const Symbol*ptr);

enum class Kind { Invalid, Literal, Ident, Assign, Label, Add, Minus, ImplicitOr, LParm, RParm,Comment,Error };
class Token  {
    QString _text;
    Kind _kind;
    int _pos;
public:
    Token(Kind kind, int pos);
    Token(Kind kind, QString text, int pos) : _kind(kind), _pos(pos),_text(text) {}
    inline QString text() const { return _text; }
    inline int len() const { return _text.length(); }
    inline int pos() const { return _pos; }
    inline  Kind kind() const { return _kind; }
};




class QPal8_Parser : public QObject
{
    Q_OBJECT
    const QString& _src;
    QList<Token> _tokens;
    typedef QList<Token>::iterator TokenIt;
    QHash<QString,Symbol*> _symtab;
public:
    explicit QPal8_Parser( QObject *parent = 0);
    void parseText(const QString& src, bool ignoreComments=true);
    const Token nextToken();
signals:

public slots:
};

#endif // QPAL8_H
