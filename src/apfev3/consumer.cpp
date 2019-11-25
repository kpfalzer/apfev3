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
    INVARIANT(n <= rem());
    Location here(*this);
    char buf[n+1];
    char c;
    for (size_t i = 0; i < n; i++) {
        c = this->operator[](i);
        buf[i] = c;
        if (c == '\n') {
            __line += 1;
            __col = 0;
        }
        __col += 1;
    }
    buf[n] = '\0';
    __pos += n;
    std::string text(buf);
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
