//
//  node.hxx
//  apfev3
//
//  Created by George P. Burdell on 12/12/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_node_hxx
#define apfev3_node_hxx

#include "apfev3/_types.hxx"

namespace apfev3 {

class _Node : public virtual TRcObj {
public:
    const TPTokens& tokens() const;
    
protected:
    explicit _Node(const TPTokens& r);
    
    virtual ~_Node() = 0;
    
private:
    TPTokens    __tokens;
};

class _Terminal : public _Node {
public:
    
protected:
    virtual ~_Terminal() = 0;
};

class _NonTerminal : public _Node {
public:
    
protected:
    virtual ~_NonTerminal() = 0;
};

}

#endif /* apfev3_node_hxx */
