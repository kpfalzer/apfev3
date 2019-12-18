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
#include <ostream>
#include "xyzzy/refcnt.hxx"
#include "apfev3/location.hxx"

namespace apfev3 {

using xyzzy::TRcObj;
using xyzzy::PTRcObjPtr;

class _Node;
class _Terminal;
class _NonTerminal;

typedef PTRcObjPtr<_Node>        TPNode;
typedef PTRcObjPtr<_Terminal>    TPTerminal;
typedef PTRcObjPtr<_NonTerminal> TPNonTerminal;

using std::ostream;

class _Node : public virtual TRcObj {
public:
    
    //token depth.  Used by alternatives to determine deepest.
    virtual size_t depth() const = 0;
    
    virtual bool isLeaf() const = 0;
    
    virtual ostream& operator<<(ostream& os) const = 0;
    
protected:
    explicit _Node()
    {}
    
    virtual ~_Node() = 0;
    
private:
};

inline
ostream& operator<<(ostream& os, const _Node& node) {
    return node.operator<<(os);
}

class _Terminal : public _Node {
public:
    const std::string text;
    const Location location;

    virtual size_t depth() const {
        return 1;
    }
    
    virtual bool isLeaf() const {
        return true;
    }

    virtual ostream& operator<<(ostream& os) const;
    
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
    
    virtual bool isLeaf() const {
        return false;
    }
    
protected:
    explicit _NonTerminal()
    : _tokenDepth(SIZE_MAX)
    {}
    
    virtual ~_NonTerminal() = 0;
    
    size_t _tokenDepth;    
};

class NodeVector : public _NonTerminal, public std::vector<TPNode> {
public:
    
    virtual size_t depth() const;
    
    virtual ~NodeVector();
    
    virtual ostream& operator<<(ostream& os) const;
    
};

typedef PTRcObjPtr<NodeVector>   TPNodeVector;

}

#endif /* apfev3_node_hxx */
