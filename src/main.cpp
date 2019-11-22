//
//  main.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <iostream>
#include "apfev3/acceptor.hpp"

int main(int argc, const char * argv[]) {
    {
        const std::string fname = __FILE__;
        apfev3::CharBuf cbuf(fname);
    }
    {
        const char* p = "12345";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        apfev3::Regex rex("\\d+");
        apfev3::TPTokens match = rex.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
    }
    std::cout << "Hello world" << std::endl;
    return(0);
}
