//
//  util.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <string>
#include <cstdio>
#include <cstdlib>
#include "apfev3/util.hpp"

namespace apfev3 {

std::string& replaceAll(std::string& s, const std::string& from, const std::string& to) {
    for (size_t pos; (pos = s.find(from)) != std::string::npos; ) {
        s.replace(pos, 1, to);
    }
    return s;
}

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

/*static*/ std::string _SList::_open = "[";
/*static*/ std::string _SList::_close = "]";
/*static*/ std::string _SList::_sep = ", ";

void _SList::open(const std::string& s) {_open = s;}
void _SList::close(const std::string& s) {_close = s;}
void _SList::sep(const std::string& s) {_sep = s;}

}
