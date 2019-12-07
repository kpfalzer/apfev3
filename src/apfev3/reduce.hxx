//
//  reduce.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 12/4/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_reduce_hpp
#define apfev3_reduce_hpp

#include <vector>
#include "apfev3/acceptor.hxx"

namespace apfev3 {

typedef std::vector<TPToken>      TokenVector;
typedef xyzzy::PTRcPtr<TokenVector>     TPTokenVector;
typedef std::vector<TPTokenVector>      TokenVectors;
typedef xyzzy::PTRcPtr<TokenVectors>    TPTokenVectors;

TPTokenVector reduce(const TPTokens& start);

std::ostream& operator<<(std::ostream& os, const TokenVector&);

std::ostream& operator<<(std::ostream& os, const TokenVectors&);

}

#endif /* apfev3_reduce_hpp */
