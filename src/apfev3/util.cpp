//
//  util.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include "apfev3/util.hpp"

namespace apfev3 {
const char* readFile(const char* const fname) {
    FILE *f = fopen(fname, "rt");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = (char *)malloc(fsize + 1);
    fread(buf, fsize, 1, f);
    fclose(f);

    buf[fsize] = 0;
    return buf;
}
}
