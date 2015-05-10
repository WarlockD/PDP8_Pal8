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
#define ADDTO(str,k) \
        { \
            QChar ch(str[0]);\
            QString s = QStringLiteral(str);\
            Pal8InternalStat temp = { s, ch, k };\
            i_kmap.insert(k,temp);\
            i_cmap.insert(ch,temp);\
        }
        ADDTO("+",Kind::Add);
        ADDTO("-",Kind::Minus);
        ADDTO("=",Kind::Assign);
        ADDTO(",",Kind::Label);
        ADDTO("#",Kind::Comment);
        ADDTO("\0",Kind::Error);
        #undef ADDTO
        return { i_kmap, i_cmap };
    }
    const QHash<Kind,Pal8InternalStat> kmap;
    const QHash<QChar,Pal8InternalStat> cmap;
};
static Pal8InternalString pal8InternalString = Pal8InternalString::create();


//uint qHash(const Symbol&ptr,int seed){ return qHash(ptr.name,seed); }
//uint qHash(const Symbol*ptr){ return qHash(ptr->name); }
//uint qHash(const Symbol*ptr,uint seed){ return qHash(ptr->name,seed); }

Token::Token(Kind kind, int pos): _kind(kind), _pos(pos), _text(pal8InternalString.kmap[kind].str) { }


QPal8_Parser::QPal8_Parser(QObject *parent) : QObject(parent)
{

}


int parseLamba(const QString& src,int pos, std::function<bool(QChar)> process ){
    int end = pos;
    while(process(src[end]) && end < src.length())  end++;
    return end;
}


Tokens QPal8_Parser::parseLine(const QString& src, bool ignoreComments){
    //QTextStream sr(src,QIODevice::OpenMode::ReadOnly);

    QChar c;
    int pos =0;
    Tokens tokens;
    int len = 0;
    Token lastTok;
    while(true) {
        if(pos >= src.length()) break;
        c = src[pos++];
        if(c == '\r' || c =='\n') break;
        if(!c.isSpace() && c != '\t') continue;

        if(c.isLetter()) {
            len = parseLamba(src,pos,[](QChar c) { return c.isLetterOrNumber();});
            lastTok=Token(Kind::Ident,src.mid(pos,len),pos);
            pos += lastTok.len();
        } else if(c.isNumber()) {
            len = parseLamba(src,pos,[](QChar c) { return c.isNumber();});
            lastTok=Token(Kind::Literal,src.mid(pos,len),pos);
            pos += lastTok.len();
        }else if(c == '#') {
            if(ignoreComments) break; // end of line if end of comments
            len = src.length() - pos;
             lastTok=Token(Kind::Comment,src.mid(pos,src.length()-pos),pos);
              pos += lastTok.len();
             break;
        } else {
            Q_ASSERT(c != QChar('\0'));
            auto it = pal8InternalString.cmap.find(c);
            if(it == pal8InternalString.cmap.end()) {
                lastTok=Token(Kind::Error,pos);
            } else {
                lastTok=Token(it->kind,it->str,pos);
            }
        }
        tokens.push_back(lastTok);
      }
    return tokens;
}


