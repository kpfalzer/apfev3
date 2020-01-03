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
    
    explicit _Terminal(const _Terminal& ref)
    : text(ref.text), location(ref.location)
    {}
    
    explicit _Terminal(const std::string& _text, const Location& _location)
    : text(_text), location(_location)
    {}
    
    explicit _Terminal(const TPNode& node);
    
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

class NodeVector;
typedef PTRcObjPtr<NodeVector>   TPNodeVector;

class NodeVector : public _NonTerminal, public std::vector<TPNode> {
public:
    explicit NodeVector(const TPNodeVector& vec);
    
    explicit NodeVector(const TPNode& node);

    explicit NodeVector()
    {}
    
    // Initialize from pattern: a (X a)*
    // to create vector of a
    void initFromOneOrMore(const TPNode& listOf);
    
    void for_each(std::function<void(const TPNode& node)> unary) const;
    
    virtual size_t depth() const;
    
    virtual ~NodeVector();
    
    virtual ostream& operator<<(ostream& os) const;
    
};

class AlternativeNode : public _Node {
public:
    explicit AlternativeNode(const TPNode& _node, size_t _selected)
    : __actual(_node), selected(_selected)
    {}
    
    //selected alternative (0-origin)
    const size_t selected;
    
    virtual ~AlternativeNode()
    {}
    
    const TPNode& actual() const {
        return __actual;
    }
    
    virtual size_t depth() const {
        return actual()->depth();
    }
    
    virtual bool isLeaf() const {
        return actual()->isLeaf();
    }
    
    virtual ostream& operator<<(ostream& os) const {
        return actual()->operator<<(os);
    }
        
private:
    const TPNode    __actual;
};

typedef PTRcObjPtr<AlternativeNode>   TPAlternativeNode;

inline TPTerminal toTerminal(const TPNode& node) {
    return xyzzy::downcast<_Node, _Terminal>(node);
}

inline TPNonTerminal toNonTerminal(const TPNode& node) {
    return xyzzy::downcast<_Node, _NonTerminal>(node);
}

inline TPNodeVector toNodeVector(const TPNode& node) {
    return xyzzy::downcast<_Node, NodeVector>(node);
}

inline TPAlternativeNode toAlternativeNode(const TPNode& node) {
    return xyzzy::downcast<_Node, AlternativeNode>(node);
}

}

#endif /* apfev3_node_hxx */
