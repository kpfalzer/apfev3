//
//  _types.hxx
//  apfev3
//
//  Created by George P. Burdell on 12/12/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <vector>
#include "xyzzy/refcnt.hxx"
#include "apfev3/util.hxx"

namespace apfev3 {

using xyzzy::TRcObj;
using xyzzy::PTRcObjPtr;
using xyzzy::PTRcPtr;

class _Node;
class _Terminal;
class _NonTerminal;
class Consumer;
class Token;
class Tokens;

typedef PTRcPtr<Token>          TPToken;
typedef PTRcPtr<Tokens>         TPTokens;
typedef SList<const TPTokens>   TokensList;
typedef PTRcPtr<TokensList>     TPTokensList;
typedef std::vector<TPToken>    TokenVector;
typedef PTRcPtr<TokenVector>    TPTokenVector;

typedef PTRcObjPtr<_Node>           TPNode;
typedef PTRcObjPtr<_Terminal>       TPTerminal;
typedef PTRcObjPtr<_NonTerminal>    TPNonTerminal;

}
