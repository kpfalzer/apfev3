//
//  node.cxx
//  apfev3
//
//  Created by George P. Burdell on 12/12/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <numeric>
#include "xyzzy/refcnt.hxx"
#include "apfev3/util.hxx"
#include "apfev3/node.hxx"

namespace apfev3 {

_Node::~_Node()
{}

_Terminal::_Terminal(const TPNode& node)
: _Terminal(dynamic_cast<const _Terminal&>(node.asT()))
{}

ostream&
_Terminal::operator<<(ostream &os) const {
    std::string text2 = text;
    replaceAll(replaceAll(text2, "\"", "\\\""), "\n", "\\n");
    os << '[' << location << ':' << text2 << ']';
    return os;
}

_Terminal::~_Terminal()
{}

_NonTerminal::~_NonTerminal()
{}

NodeVector::~NodeVector()
{}

ostream&
NodeVector::operator<<(ostream &os) const {
    size_t i = 0;
    for (auto iter = cbegin(); iter != cend(); ++iter, ++i) {
        if (0 < i) os << ", ";
        (*iter)->operator<<(os);
    }
    return os;
}

size_t
NodeVector::depth() const {
    //cache up the value
    if (SIZE_MAX == _tokenDepth) {
        size_t depth = std::accumulate(begin(), end(), 0,
                               [](size_t sum, const TPNode& node){
            return sum + node->depth();
        });
        const_cast<NodeVector*>(this)->_tokenDepth = depth;
    }
    return _tokenDepth;
}

}
