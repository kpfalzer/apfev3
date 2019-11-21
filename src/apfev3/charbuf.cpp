//
//  charbuf.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <cstdlib>
#include "apfev3/util.hpp"
#include "apfev3/charbuf.hpp"

namespace apfev3 {
CharBuf::CharBuf(const std::string fname)
: fileName(fname),
__buf(readFile(fname)),
__length(std::strlen(__buf)),
__owner(true)
{}

CharBuf::CharBuf(const char* const buf)
: fileName("_"),
__buf(buf),
__length(strlen(buf)),
__owner(false)
{}

CharBuf::CharBuf(const CharBuf& from)
: fileName(from.fileName),
__buf(from.__buf),
__length(from.__length),
__owner(false)
{}

void CharBuf::__cleanup() {
    if (__owner) {
        std::free((void*)__buf);
    }
}
const CharBuf& CharBuf::operator=(const CharBuf& from) {
    __cleanup();
    const_cast<std::string&>(fileName) = from.fileName;
    __buf = from.__buf;
    __length = from.__length;
    __owner = false;
    return *this;
}

CharBuf::~CharBuf() {
    __cleanup();
}
}
