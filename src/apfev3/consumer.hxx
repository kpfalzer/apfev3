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

namespace apfev3 {

class Token;
class Location;
class Mark;

typedef xyzzy::PTRcPtr<Token>  TPToken;

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

    typedef SList<Consumer> ConsumerList;
    typedef xyzzy::PTRcPtr<ConsumerList> TPConsumerList;
    
    TPConsumerList& alts() {
        return __alts;
    }
    
    bool hasAlts() const {
        return __alts.isValid() && !__alts->isEmpty();
    }
    
    TPConsumerList& addAlt(const Consumer& consumer);
    
    void replaceAlts(TPConsumerList& from);
    
    bool operator==(const Consumer& other) const;
    
    bool operator!=(const Consumer& other) const {
        return !this->operator==(other);
    }
    
private:
    // Use method rewind() to be more explicit
    const Consumer& operator=(const Consumer& from);
    
    TPConsumerList __alts;
    const CharBuf&  __buf;
    size_t __pos;
    size_t __line, __col;
};

class Location {
public:
    Location(const Consumer& here)
    : filename(here.filename()),line(here.__line), col(here.__col)
    {}
    
    explicit Location(const std::string& filename, size_t line, size_t col)
    : filename(filename), line(line), col(col) {}
    
    static const std::string UNKNOWN;
    
    explicit Location()
    : filename(UNKNOWN), line(0), col(0) {}
    
    //allow default copy constructors and destructor

    const std::string& filename;
    const size_t    line, col;
    
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
    
class Token {
public:
    explicit Token(const std::string& _text, const Location& _loc)
    : text(_text), location(_loc)
    {}
    
    explicit Token(const std::string& _text, const Consumer& here)
    : text(_text), location(here)
    {}
    
    explicit Token() {}
    
    explicit Token(const Token& from)
    : text(from.text),
    location(from.location)
    {}
    
    virtual ~Token();
    
    virtual std::ostream& operator<<(std::ostream& os) const;

    const std::string text;
    const Location location;
};

class Mark {
public:
    explicit Mark(size_t _pos, size_t _line, size_t _col)
    : pos(_pos), line(_line), col(_col)
    {}
    
    explicit Mark()
    : Mark(0,0,0)
    {}

    const Mark& operator=(const Mark& from);
    
    const size_t pos, line, col;
};

typedef Consumer::ConsumerList      ConsumerList;
typedef Consumer::TPConsumerList    TPConsumerList;

// Append consumer to list if not exist
void append(TPConsumerList& list, const Consumer& consumer);

inline
std::ostream& operator<<(std::ostream& os, const Location& ele) {
    return ele.operator<<(os);
}

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
