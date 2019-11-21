//
//  main.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <iostream>
#include "apfev3/charbuf.hpp"
#include "apfev3/util.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    const std::string fname = __FILE__;
    apfev3::CharBuf cbuf(fname);
    apfev3::CharBuf cbuf2(cbuf);
    //INVARIANT(3 == 4);
    return 0;
}
