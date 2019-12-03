//
//  charbuf.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_charbuf_hxx
#define apfev3_charbuf_hxx

#include <string>

namespace apfev3 {
class CharBuf {
public:
    explicit CharBuf(const std::string fname);

    explicit CharBuf(const char* const buf);
    
    explicit CharBuf(const CharBuf& from);
    
    const CharBuf& operator=(const CharBuf& from);
    
    virtual ~CharBuf();
    
    char operator[](size_t i) const {
        return (i < length()) ? __buf[i] : '\0';
    }
    
    size_t length() const {
        return __length;
    }
    
    const std::string fileName;
    
private:
    void __cleanup();
    
    const char* __buf;
    size_t __length;
    bool __owner;
};
}

#endif /* apfev3_charbuf_hxx */
