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
#include "apfev3/charbuf.hxx"
#include "apfev3/util.hxx"

namespace apfev3 {
class Consumer {
public:
    explicit Consumer(const CharBuf& buf);
    
    explicit Consumer(const Consumer& from);
    
    struct Mark;
    
    char operator[](size_t n) const {
        return la(n);
    }
    
    char la(size_t n) const {
        return __buf[__pos + n];
    }
    
    friend struct Location;
    
    struct Location {
        explicit Location(const Consumer& here)
        : filename(here.filename()),line(here.__line), col(here.__col), pos(here.__pos)
        {}
        
        explicit Location(const std::string& filename, size_t line, size_t col, size_t pos)
        : filename(filename), line(line), col(col), pos(pos) {}
        
        //allow default copy constructors and destructor

        const std::string& filename;
        const size_t    line, col, pos;
        
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
    
    // accept n chars; but token is [start,end]
    const TPToken accept(size_t n, size_t start, size_t end);
    
    bool isEOF(size_t offset = 0) const {
        return (0 >= rem(offset));
    }
    
    size_t rem(size_t offset = 0) const {
        return __buf.length() - (__pos + offset);
    }
    
    Mark mark() const {
        return Mark(__pos, __line, __col);
    }
    
    void rewind(const Mark& mark);
    
    void rewind(const Consumer& from);
    
    TPLocation location() const {
        return new Location(*this);
    }
    
    const std::string& filename() const {
        return __buf.fileName;
    }
    
    virtual std::ostream& operator<<(std::ostream& os) const;

    typedef SList<Consumer> ConsumerList;
    typedef SingleOwnerPtr<ConsumerList> TPConsumerList;
    
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

typedef Consumer::ConsumerList ConsumerList;
typedef Consumer::TPConsumerList TPConsumerList;

// Append consumer to list if not exist
void append(TPConsumerList& list, const Consumer& consumer);

inline
std::ostream& operator<<(std::ostream& os, const Consumer::Location& ele) {
    return ele.operator<<(os);
}

inline
std::ostream& operator<<(std::ostream& os, const Consumer::Token& ele) {
    return ele.operator<<(os);
}

inline
std::ostream& operator<<(std::ostream& os, const Consumer& ele) {
    return ele.operator<<(os);
}

}

#endif /* apfev3_consumer_hxx */
