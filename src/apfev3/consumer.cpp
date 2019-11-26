//
//  consumer.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <algorithm>
#include "apfev3/util.hpp"
#include "apfev3/consumer.hpp"

namespace apfev3 {

Consumer::Consumer(const CharBuf& buf)
: __buf(buf),
__pos(0),
__line(1),
__col(1)
{}

const Consumer::TPToken Consumer::accept(size_t n) {
    return accept(n,0,n-1);
}

const Consumer::TPToken Consumer::accept(size_t n, size_t start, size_t end) {
    INVARIANT(n <= rem());
    INVARIANT(end >= start);
    size_t line = 0, col = 0;
    const size_t nn = 1+end-start;
    INVARIANT(0 < nn);
    char buf[nn];
    char c;
    for (size_t i = 0; i < n; i++) {
        c = this->operator[](i);
        if ((i >= start) && (i <= end)) {
            buf[i] = c;
            if (0 == line) {
                line = __line;
                col = __col;
            }
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

const Consumer::Mark&
Consumer::Mark::operator=(const Mark& from) {
    const_cast<size_t&>(pos)  = from.pos;
    const_cast<size_t&>(line)  = from.line;
    const_cast<size_t&>(col)  = from.col;
    return *this;
}

Consumer::Token::~Token() {
}
 
std::ostream&
Consumer::Location::operator<<(std::ostream& os) const {
    os << filename << ":" << line << ":" << col;
    return os;
}

std::ostream&
Consumer::Token::operator<<(std::ostream& os) const {
    std::string s = text;
    replaceAll(replaceAll(s, "\"", "\\\""), "\n", "\\n");
    os << location << ":" << '"' << s << '"';
    return os;
}

}
