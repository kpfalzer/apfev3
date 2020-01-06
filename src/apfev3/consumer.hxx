//
//  consumer.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_consumer_hxx
#define apfev3_consumer_hxx

#include <ostream>
#include <cstdlib>
#include "xyzzy/refcnt.hxx"
#include "apfev3/charbuf.hxx"
#include "apfev3/util.hxx"
#include "apfev3/node.hxx"

namespace apfev3 {

class Token;
class Location;
class Mark;

typedef xyzzy::PTRcObjPtr<Token>  TPToken;
 
TPNode toNode(const TPToken& token);

class Consumer {
public:
    explicit Consumer(const CharBuf& buf);
    
    explicit Consumer(const Consumer& from);
    
    char operator[](size_t n) const {
        return la(n);
    }
    
    char la(size_t n) const {
        return __buf[__pos + n];
    }
    
    friend struct Location;
    
    // accept n chars
    TPToken accept(size_t n);
    
    // accept n chars; but token is [start,end]
    TPToken accept(size_t n, size_t start, size_t end);
    
    bool isEOF(size_t offset = 0) const {
        return (0 >= rem(offset));
    }
    
    size_t rem(size_t offset = 0) const {
        return __buf.length() - (__pos + offset);
    }
    
    Mark mark() const;
    
    void rewind(const Mark& mark);
    
    void rewind(const Consumer& from);
    
    Location location() const;
    
    const std::string& filename() const {
        return __buf.fileName;
    }
    
    virtual std::ostream& operator<<(std::ostream& os) const;

    bool operator==(const Consumer& other) const;
    
    bool operator!=(const Consumer& other) const {
        return !this->operator==(other);
    }
    
private:
    // Use method rewind() to be more explicit
    const Consumer& operator=(const Consumer& from);
    
    const CharBuf&  __buf;
    size_t __pos;
    size_t __line, __col;
};

class Token : public virtual _Terminal {
public:
    explicit Token(const std::string& _text, const Location& _loc)
    : _Terminal(_text, _loc)
    {}
    
    explicit Token(const std::string& _text, const Consumer& here)
    : _Terminal(_text, here.location())
    {}
    
    explicit Token() {}
    
    explicit Token(const Token& from)
    : _Terminal(from.text, from.location)
    {}
    
    virtual ~Token();
};

class Mark {
public:
    explicit Mark(size_t _pos, size_t _line, size_t _col)
    : pos(_pos), line(_line), col(_col)
    {}
    
    explicit Mark()
    : Mark(0,0,0)
    {}

    size_t operator-(const Mark& to) {
        return pos - to.pos;
    }
    
    const Mark& operator=(const Mark& from);
    
    const size_t pos, line, col;
};

inline
std::ostream& operator<<(std::ostream& os, const Token& ele) {
    return ele.operator<<(os);
}

inline
std::ostream& operator<<(std::ostream& os, const Consumer& ele) {
    return ele.operator<<(os);
}

}

#endif /* apfev3_consumer_hxx */
