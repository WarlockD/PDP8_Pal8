#include "qpal8.h"
#include <QHash>
#include <QException>
#include <QTextStream>
#include <QDebug>
#include <type_traits>
#include <functional>

struct Pal8InternalStat {
    QString str;
    QChar c;
    Kind kind;
    bool operator==(const Pal8InternalStat& o) { return kind == o.kind; }
};
inline uint qHash(const Kind& o, uint seed=0) { return qHash((int)o,seed); }
inline uint qHash(const Pal8InternalStat& o, uint seed=0) { return qHash((int)o.kind,seed); }

/*
#  define QStringLiteral(str) \
    ([]() -> QString { \
        enum { Size = sizeof(QT_UNICODE_LITERAL(str))/2 - 1 }; \
        static const QStaticStringData<Size> qstring_literal = { \
            Q_STATIC_STRING_DATA_HEADER_INITIALIZER(Size), \
            QT_UNICODE_LITERAL(str) }; \
        QStringDataPtr holder = { qstring_literal.data_ptr() }; \
        const QString qstring_literal_temp(holder); \
        return qstring_literal_temp; \
    }()) \
    /**/


struct Pal8InternalString {
    static Pal8InternalString create() {
        QHash<Kind,Pal8InternalStat> i_kmap;
        QHash<QChar,Pal8InternalStat> i_cmap;
        QHash<Kind,QString> i_emap;
#define ADDTO(str,k) \
        { \
            QChar ch(str[0]);\
            QString s = QStringLiteral(str);\
            Pal8InternalStat temp = { s, ch, k };\
            i_kmap.insert(k,temp);\
            i_cmap.insert(ch,temp);\
            i_emap.insert(k,QStringLiteral("%%k"));\
        }
        ADDTO("+",Kind::Add);
        ADDTO("-",Kind::Minus);
        ADDTO("=",Kind::Assign);
        ADDTO(",",Kind::Label);
        ADDTO("#",Kind::Comment);
        ADDTO("\0",Kind::Error);
        ADDTO("\0",Kind::Literal);
        ADDTO("\0",Kind::Ident);
        #undef ADDTO
        return { i_kmap, i_cmap, i_emap };
    }
    const QHash<Kind,Pal8InternalStat> kmap;
    const QHash<QChar,Pal8InternalStat> cmap;
    const QHash<Kind,QString> emap;
};
static Pal8InternalString pal8InternalString = Pal8InternalString::create();


//uint qHash(const Symbol&ptr,int seed){ return qHash(ptr.name,seed); }
//uint qHash(const Symbol*ptr){ return qHash(ptr->name); }
//uint qHash(const Symbol*ptr,uint seed){ return qHash(ptr->name,seed); }


 QString Token::debug() const {
     //QString o = QString("(%1 : %2 : %3 : %4)").arg(pal8InternalString.emap[kind()]).arg(_text.position()).arg(_text.length()).arg(text());
     return "";
 }
 int parseLamba(const QString& src,int pos, std::function<bool(QChar)> process ){
     int start = pos;
     while(pos < src.length() && process(src[pos]) )  pos++;
     return pos-start;
 }
 TokenLine::TokenLine(const QString& src, int lineno) : _line(src), _lineno(lineno) {
     setLine(src,lineno);
 }
void TokenLine::setLine(const QString& str, int lineno) {
    _tokens.clear();
    _line = str;
    _lineno = lineno;
    QChar c;
    int pos =0;
   // Tokens tokens;
    int len = 0;
    Token lastTok;
    while(true) {
        if(pos >= _line.length()) break;
        c = _line[pos];
        if(c == '\r' || c =='\n') break;
        if(c.isSpace() || c == '\t') { pos++; continue; }

        if(c.isLetter()) {
            len = parseLamba(_line,pos,[](QChar c) { return c.isLetterOrNumber();});
            lastTok=Token(Kind::Ident,QStringRef(&_line,pos,len));
            pos += lastTok.len();
        } else if(c.isNumber()) {
            len = parseLamba(_line,pos,[](QChar c) { return c.isNumber();});
            lastTok=Token(Kind::Literal,QStringRef(&_line,pos,len));
            pos += lastTok.len();
        }else if(c == '#') {
            // if(ignoreComments) break; // end of line if end of comments
             lastTok=Token(Kind::Comment,QStringRef(&_line,pos,_line.length()-pos));
              pos += lastTok.len();
        } else {
            Q_ASSERT(c != QChar('\0'));
            auto it = pal8InternalString.cmap.find(c);
            if(it == pal8InternalString.cmap.end()) {
                lastTok=Token(Kind::Error,QStringRef(&_line,pos,1));
            } else {
                lastTok=Token(it->kind,QStringRef(&_line,pos,1));
            }
            pos++;
        }
        _tokens.push_back(lastTok);
      }
}

QPal8_Parser::QPal8_Parser(QObject *parent) : QObject(parent)
{

}



 Tokens QPal8_Parser::assemble(const QString& src) {
     (void)src;

    return Tokens();
 }


Tokens QPal8_Parser::parseLine(const QString& src, bool ignoreComments){
    //QTextStream sr(src,QIODevice::OpenMode::ReadOnly);
    (void)src,ignoreComments;
    return Tokens();
}


