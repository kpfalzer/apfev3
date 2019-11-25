//
//  consumer.hpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_consumer_hpp
#define apfev3_consumer_hpp

#include <ostream>
#include <cstdlib>
#include "apfev3/charbuf.hpp"
#include "apfev3/util.hpp"

namespace apfev3 {
class Consumer {
public:
    explicit Consumer(const CharBuf& buf);
    
    //allow default copy constructors and destructor
    
    char operator[](size_t n) const {
        return la(n);
    }
    
    char la(size_t n) const {
        return __buf[__pos + n];
    }
    
    friend struct Location;
    
    struct Location {
        explicit Location(const Consumer& here)
        : filename(here.filename()),line(here.__line), col(here.__col)
        {}
        
        //allow default copy constructors and destructor

        const std::string& filename;
        const size_t    line, col;
        
        virtual std::ostream& operator<<(std::ostream& os) const;
        
    };
    
    typedef SingleOwnerPtr<Location> TPLocation;
    
    struct Token {
        explicit Token(const std::string& _text, const Location& _loc)
        : text(_text), location(new Location(_loc))
        {}
        
        explicit Token(const std::string& _text, const Consumer& here)
        : text(_text), location(new Location(here))
        {}
        
        explicit Token() : location(nullptr){}
        
        explicit Token(const Token& from)
        : text(from.text),
        location(from.location)
        {}
        
        virtual ~Token();
        
        virtual std::ostream& operator<<(std::ostream& os) const;

        const std::string text;
        const TPLocation location;
    };
    
    typedef SingleOwnerPtr<Token>  TPToken;
    
    struct Mark {
        explicit Mark(size_t _pos, size_t _line, size_t _col)
        : pos(_pos), line(_line), col(_col)
        {}
        
        explicit Mark()
        : Mark(0,0,0)
        {}

        const Mark& operator=(const Mark& from);
        
        const size_t pos, line, col;
    };
    
    // accept n chars
    const TPToken accept(size_t n);
    
    bool isEOF() const {
        return (0 == rem());
    }
    
    size_t rem() const {
        return __buf.length() - __pos;
    }
    
    Mark mark() const {
        return Mark(__pos, __line, __col);
    }
    
    void rewind(const Mark& mark) {
        __pos = mark.pos;
        __line = mark.line;
        __col = mark.col;
    }
    
    TPLocation location() const {
        return new Location(*this);
    }
    
    const std::string& filename() const {
        return __buf.fileName;
    }
private:
    const CharBuf&  __buf;
    size_t __pos;
    size_t __line, __col;
};

inline
std::ostream& operator<<(std::ostream& os, const Consumer::Location& ele) {
    return ele.operator<<(os);
}

inline
std::ostream& operator<<(std::ostream& os, const Consumer::Token& ele) {
    return ele.operator<<(os);
}

}

#endif /* apfev3_consumer_hpp */
