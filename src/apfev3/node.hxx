//
//  node.hxx
//  apfev3
//
//  Created by George P. Burdell on 12/12/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_node_hxx
#define apfev3_node_hxx

#include <vector>
#include "xyzzy/refcnt.hxx"
#include "apfev3/location.hxx"

namespace apfev3 {

using xyzzy::TRcObj;

class _Node : public virtual TRcObj {
public:
    
protected:
    explicit _Node()
    {}
    
    virtual ~_Node() = 0;
    
private:
};

class _Terminal : public _Node {
public:
    const std::string text;
    const Location location;

protected:
    explicit _Terminal()
    {}
    
    explicit _Terminal(const std::string& _text, const Location& _location)
    : text(_text), location(_location)
    {}
    
    virtual ~_Terminal() = 0;
};

class _NonTerminal : public _Node {
public:
    
protected:
    explicit _NonTerminal()
    {}
    
    virtual ~_NonTerminal() = 0;
};

class NodeVector : public _NonTerminal, public std::vector<_Node> {
public:
    
    virtual ~NodeVector();
};

typedef xyzzy::PTRcObjPtr<_Node>        TPNode;
typedef xyzzy::PTRcObjPtr<_Terminal>    TPTerminal;
typedef xyzzy::PTRcObjPtr<_NonTerminal> TPNonTerminal;

typedef xyzzy::PTRcObjPtr<NodeVector>   TPNodeVector;

}

#endif /* apfev3_node_hxx */
