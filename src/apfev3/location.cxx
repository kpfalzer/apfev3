//
//  location.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "apfev3/util.hxx"
#include "apfev3/location.hxx"

namespace apfev3 {

/*static*/ const std::string Location::UNKNOWN = "<unintialized>";

const Location&
Location::operator=(const Location& from) {
    INVARIANT(filename == UNKNOWN);
    const_cast<std::string&>(filename) = from.filename;
    const_cast<size_t&>(line) = from.line;
    const_cast<size_t&>(col) = from.col;
    return *this;
}


bool
Location::operator==(const Location& other) const {
    INVARIANT(filename == other.filename);
    return (line == other.line) && (col == other.col);
}

bool
Location::operator<(const Location& other) const {
    INVARIANT(filename == other.filename);
    return (line < other.line) || ((line == other.line) && (col < other.col));
}

bool
Location::operator>(const Location& other) const {
    INVARIANT(filename == other.filename);
    return (line > other.line) || ((line == other.line) && (col > other.col));
}

std::ostream&
Location::operator<<(std::ostream& os) const {
    os << filename << ":" << line << ":" << col;
    return os;
}

}
