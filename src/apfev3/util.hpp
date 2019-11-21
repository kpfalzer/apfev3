//
//  util.hpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_util_hpp
#define apfev3_util_hpp

#include <string>

namespace apfev3 {
const char* readFile(const char* const fname);

inline const char* readFile(const std::string fname) {
    return readFile(fname.c_str());
}

inline void invariant(bool cond, const char* message) {
    if (!cond) {
        throw std::logic_error(message);
    }
}

}

#define INVARIANT(_expr) apfev3::invariant(_expr, "Invariant failed: " #_expr)

#endif /* apfev3_util_hpp */
