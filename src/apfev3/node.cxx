//
//  node.cxx
//  apfev3
//
//  Created by George P. Burdell on 12/12/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <algorithm>
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

void
_Terminal::_set(const std::string& _text, const Location& _location) {
    INVARIANT(!location.isValid());
    const_cast<std::string&>(text) = _text;
    const_cast<Location&>(location) = _location;
}

_NonTerminal::~_NonTerminal()
{}

void
NodeVector::initFromOneOrMore(const TPNode& listOf) {
    TPNodeVector nodes = toNodeVector(listOf);
    this->push_back(nodes->at(0));
    nodes = toNodeVector(nodes->at(1));
    nodes->for_each([this](const TPNode& node){
        TPNodeVector seq = toNodeVector(node);
        this->push_back(seq->at(1));
    });
    this->shrink_to_fit();
}

void
NodeVector::for_each(std::function<void(const TPNode& node)> unary) const {
    std::for_each(cbegin(), cend(), unary);
}

NodeVector::~NodeVector()
{}

ostream&
NodeVector::operator<<(ostream &os) const {
    size_t i = 0;
    // We don't use comma since we now use << to regurgitate input stream
    for (auto iter = cbegin(); iter != cend(); ++iter, ++i) {
        if (0 < i) os << ' '; //", ";
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

NodeVector::NodeVector(const TPNodeVector& vec)
: vector<TPNode>(vec.asT())
{}

NodeVector::NodeVector(const TPNode& node)
: NodeVector(toNodeVector(node))
{}

}
