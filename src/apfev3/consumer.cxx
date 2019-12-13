//
//  consumer.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <algorithm>
#include "apfev3/util.hxx"
#include "apfev3/location.hxx"
#include "apfev3/consumer.hxx"

namespace apfev3 {

Consumer::Consumer(const CharBuf& buf)
: __buf(buf),
__pos(0),
__line(1),
__col(1)
{}

Consumer::Consumer(const Consumer& from)
: __buf(from.__buf),
__pos(from.__pos),
__line(from.__line),
__col(from.__col)
{}

// Append consumer to list if not exist
void append(TPConsumerList& list, const Consumer& consumer) {
    bool noMatch = true;
    if (list.isNull()) {
        list = new ConsumerList();
    } else {
        for (auto iter = list->iterator(); noMatch && iter.hasMore(); ) {
            if (iter.next() == consumer) {
                noMatch = false;
            }
        }
    }
    if (noMatch) {
        list->append(consumer);
    }
}

TPConsumerList& Consumer::addAlt(const Consumer& consumer) {
    if (*this != consumer) {
        append(alts(), consumer);
    }
    return alts();
}

void Consumer::replaceAlts(TPConsumerList& from) {
    if (from.isNull() || from->isEmpty()) {
        __alts = nullptr;
    } else {
        __alts = from;
    }
}

bool Consumer::operator==(const Consumer& other) const {
    return (&__buf == &other.__buf) && (__pos == other.__pos);
}

void Consumer::rewind(const Mark& mark) {
    __pos = mark.pos;
    __line = mark.line;
    __col = mark.col;
}

void Consumer::rewind(const Consumer& from) {
    __pos = from.__pos;
    __line = from.__line;
    __col = from.__col;
}

Mark Consumer::mark() const {
    return Mark(__pos, __line, __col);
}

Location Consumer::location() const {
    return Location(this->filename(), this->__line, this->__col);
}

TPToken Consumer::accept(size_t n) {
    return accept(n,0,n-1);
}

TPToken Consumer::accept(size_t n, size_t start, size_t end) {
    INVARIANT(n <= rem());
    INVARIANT(end >= start);
    const size_t line = __line, col = __col;
    const size_t nn = 1+end-start;
    INVARIANT(0 < nn);
    char buf[nn];
    char c;
    for (size_t i = 0; i < n; i++) {
        c = this->operator[](i);
        if ((i >= start) && (i <= end)) {
            buf[i] = c;
        }
        if (c == '\n') {
            __line += 1;
            __col = 0;
        }
        __col += 1;
    }
    buf[nn] = '\0';
    __pos += n;
    std::string text(buf);
    Location here(this->filename(), line, col);
    return new Token(text, here);
}

const Mark&
Mark::operator=(const Mark& from) {
    const_cast<size_t&>(pos)  = from.pos;
    const_cast<size_t&>(line)  = from.line;
    const_cast<size_t&>(col)  = from.col;
    return *this;
}

Token::~Token() {
}
 
std::ostream&
Consumer::operator<<(std::ostream& os) const {
    os << filename() << ":" << __line << ":" << __col << '[' << __pos << ']';
    return os;
}

std::ostream&
Token::operator<<(std::ostream& os) const {
    std::string s = text;
    replaceAll(replaceAll(s, "\"", "\\\""), "\n", "\\n");
    os << location << ":" << '"' << s << '"';
    return os;
}

}
