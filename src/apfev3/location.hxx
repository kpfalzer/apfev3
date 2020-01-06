//
//  location.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_location_hxx
#define apfev3_location_hxx

#include <ostream>
#include <cstdlib>

namespace apfev3 {

class Location {
public:
    explicit Location(const std::string& filename, size_t line, size_t col)
    : filename(filename), line(line), col(col) {}
    
    const Location& operator=(const Location& from);
    
    static const std::string UNKNOWN;
    
    explicit Location()
    : filename(UNKNOWN), line(0), col(0) {}
    
    //allow default copy constructors and destructor

    const std::string& filename;
    const size_t    line, col;
    
    bool isValid() const {
        return UNKNOWN != filename;
    }
    
    virtual std::ostream& operator<<(std::ostream& os) const;
    
    bool operator==(const Location& other) const;
    
    bool operator<(const Location& other) const;
    
    bool operator>(const Location& other) const;
    
    bool operator!=(const Location& other) const {
        return !this->operator==(other);
    }
    
    bool operator<=(const Location& other) const {
        return this->operator<(other) || this->operator==(other);
    }
    
    bool operator>=(const Location& other) const {
        return this->operator>(other) || this->operator==(other);
    }
};

inline
std::ostream& operator<<(std::ostream& os, const Location& ele) {
    return ele.operator<<(os);
}

}

#endif /* apfev3_location_hxx */
