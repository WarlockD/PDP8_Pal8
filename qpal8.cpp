#include "qpal8.h"
#include <QException>
#include <QTextStream>
struct Pal8InternalStat {
    QString str;
    QChar c;
    Kind kind;
};

struct Pal8InternalString {
    const QHash<Kind,Pal8InternalStat> kmap;
    const QHash<QChar,Pal8InternalStat> cmap;
    Pal8InternalString() {
        cmap['+']  = kmap[Kind::Add] = QStringLiteral("+");
        cmap['-']  = kmap[Kind::Minus] = QStringLiteral("-");
        cmap['=']  = kmap[Kind::Assign] = QStringLiteral("=");
        cmap[',']  = kmap[Kind::Label] = QStringLiteral(",");
        cmap['#']  = kmap[Kind::Comment] = QStringLiteral("#");
    }
};
static Pal8InternalString pal8InternalString;


uint qHash(const Symbol&ptr){ return qHash(ptr.name); }
uint qHash(const Symbol*ptr){ return qHash(ptr->name); }

Token::Token(Kind kind, int pos): _kind(kind), _pos(pos) { _text = pal8InternalString.kmap[kind]; }


QPal8_Parser::QPal8_Parser(QObject *parent) : QObject(parent)
{

}
inline static bool skipWhiteSpace(const QString& src, int&pos, QChar& c) {
    while(true) {

        pos++;
    }
    return false;
}
int parseWord(const QString& src,int pos) {
    int end = pos;
    while(src[end].isLetterOrNumber() || end < src.length())  end++;
    return end;
}

int parseLamba(const QString& src,int pos, std::function<bool(QChar)> process ){
    int end = pos;
    while(process(src[end]) && end < src.length())  end++;
    return end;
}


void QPal8_Parser::parseText(const QString& src, bool ignoreComments){
    QTextStream sr(src,QIODevice::OpenMode::ReadOnly);

    QChar c;
    int pos =0;
    _tokens.clear();
    enum class State { Start, Normal, LastIdent, LastLiteral, LastAssign, LastLabel, LastOperator };
    State state = State::Start;
    int len = 0;
    Token lastTok;
    while(true) {
        if(pos >= src.length()) break;
        c = src[pos++];
        if(!c.isSpace() && c != '\t') continue;
        if(c == '\r' || c =='\n') break;
        if(c.isLetter()) {
            len = parseLamba(src,pos,[](QChar c) { return c.isLetterOrNumber()});
            lastTok=Token(Kind::Ident,src.mid(pos,len),pos);
            pos += lastTok.len();
            _tokens.push_back(lastTok);
            continue;
        }
        if(c.isNumber()) {
            len = parseLamba(src,pos,[](QChar c) { return c.isNumber()});
            lastTok=Token(Kind::Literal,src.mid(pos,len),pos);
            pos += lastTok.len();
            _tokens.push_back(lastTok);
            continue;
        }
        if(c == '#') {
            if(ignoreComments) break; // end of line if end of comments
            len = src.length() - pos;
             lastTok=Token(Kind::Comment,src.mid(pos,len),pos);
              pos += lastTok.len();
             _tokens.push_back(lastTok);
             break;
        }
        auto it = pal8InternalString.cmap.find(c);
        if(it == pal8InternalString.cmap.end()) {
            lastTok=Token(Kind::Comment,pos,len);
        }
      // if(pal8InternalString)
      }
}

Token QPal8_Parser::nextToken(bool ignoreComments) {
    if(_pos >= _src.length()) return Token(Kind::EoS);
    QChar c= _src[_pos++];
    int startPos = _pos;
    while(pos < _src.length()) {
        c = _src[_pos++];
    }
    QString temp;
    int startIndex, endIndex;
    int value;
    enum class State { Start, Keyword, Number, Operator,Assign };
    State state = State::Start;
    for(int pos = 0; pos < text.length();pos++) {
        c = text[pos];
        switch(state) {
        case State::Start:
            startIndex = pos;
            if(c.isLetter()) state = State::Keyword;
            if(c.isDigit()) state = State::Number;
            if(c == '=') state = State::Assign;


        }


    }
}

