//
//  acceptor.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "xyzzy/refcnt.hxx"
#include "apfev3/util.hxx"
#include "apfev3/acceptor.hxx"

namespace apfev3 {

using xyzzy::downcast;
using xyzzy::upcast;

TPNode
_Acceptor::accept(Consumer& consumer) const {
    return __accept(consumer);
}

TPNode
_Acceptor::__accept(Consumer& consumer) const {
    return (!_checksForEOF() && consumer.isEOF()) ? nullptr : _accept(consumer);
}

_Acceptor::~_Acceptor() {}

Regex::Regex(const std::string& pattern)
: _rex(pattern.c_str()) {}
 
TPNode
Regex::_accept(Consumer& consumer) const {
    std::string s = "";
    size_t n = 0;
    for (; n < consumer.rem(); n++) {
        s += consumer[n];
        if (! std::regex_match(s, _rex)) {
            break;
        }
    }
    return (0 < n) ? upcast<_Node>(_create(consumer, s.substr(0,n))) : nullptr;
}

TPToken
Regex::_create(Consumer& consumer, const std::string& text) const {
    return consumer.accept(text.length());
}

TPToken
Regex::_skipTrailingWs(Consumer& consumer, const std::string& text) const {
    std::smatch match;
    INVARIANT(std::regex_match(text, match, _rex));
    const std::string ident = match[1];
    return consumer.accept(text.length(), 0, match[1].length()-1);
}

TPNode
Repetition::_accept(Consumer& consumer) const {
    TPNodeVector nodes;
    while (true) {
        auto p = __ele.accept(consumer);
        if (p.isNull()) break;
        if (nodes.isNull()) {
            nodes = new NodeVector();
        }
        nodes->push_back(p);
    }
    if (nodes.isNull()) {
        switch (type) {
            case eOptional:
            case eZeroOrMore:
                nodes = new NodeVector();
                break;
            default:
                ;//do nothing
        }
    }
    return (nodes.isValid()) ? upcast<_Node>(nodes) : nullptr;
}

TPNode
Sequence::_accept(Consumer& consumer) const {
    TPNodeVector nodes;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        auto p = iter.next()->accept(consumer);
        if (p.isNull()) return nullptr;
        if (nodes.isNull()) {
            nodes = new NodeVector();
        }
        nodes->push_back(p);
    }
    INVARIANT(nodes.isValid());
    return upcast<_Node>(nodes);
}

Sequence::~Sequence()
{}

TPNode
Alternatives::_accept(Consumer& consumer) const {
    const Consumer start(consumer);
    size_t n = 0;
    TPNode longest;
    Mark advance;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        Consumer xconsumer(start);
        auto p = iter.next()->accept(xconsumer);
        if (!p.isValid()) continue;
        const size_t m = p->depth();
        if (m > n) {
            longest = p;
            n = m;
            advance = xconsumer.mark();
        }
    }
    if (longest.isValid()) {
        consumer.rewind(advance);
    } else {
        consumer.rewind(start);
    }
    return longest;
}

Alternatives::~Alternatives()
{}

namespace token {

static const std::string __IDENT_PATT = "([_a-zA-Z][_a-zA-Z\\d]*)\\s*";
Ident::Ident() : Ident(__IDENT_PATT) {}
Ident::Ident(const std::string& patt) : Regex(patt) {}
TPToken Ident::_create(Consumer& consumer, const std::string& text) const {
    return _skipTrailingWs(consumer, text);
}

/*static*/ const Ident& Ident::THE_ONE = Ident();
 
TPNode
LineComment::_accept(Consumer& consumer) const {
    if (consumer[0] != '/' || consumer[1] != '/')
        return nullptr;
    size_t n = 2;
    for (; consumer[n] != '\n' && !consumer.isEOF(n); ++n)
        ;
    if (consumer.isEOF(n))
        --n;
    return toNode(consumer.accept(n+1));
}

/*static*/ const LineComment& LineComment::THE_ONE = LineComment();

TPNode
BlockComment::_accept(Consumer& consumer) const {
    if (consumer[0] != '/' || consumer[1] != '*')
        return nullptr;
    size_t n = 2;
    for (; !(consumer[n] == '*' && consumer[n+1] == '/') && !consumer.isEOF(n); ++n)
        ;
    INVARIANT(!consumer.isEOF(n));
    return toNode(consumer.accept(n+2));
}

/*static*/ const BlockComment& BlockComment::THE_ONE = BlockComment();

static const std::string __WHITESPACE_PATT = "\\s+";
WhiteSpace::WhiteSpace() : WhiteSpace(__WHITESPACE_PATT) {}
WhiteSpace::WhiteSpace(const std::string& patt) : Regex(patt) {}

/*static*/ const WhiteSpace& WhiteSpace::THE_ONE = WhiteSpace();

static const Alternatives __SPACING_ALTS({&LineComment::THE_ONE,
                                       &BlockComment::THE_ONE,
                                       &WhiteSpace::THE_ONE});
static const Repetition __SPACING(__SPACING_ALTS, Repetition::eZeroOrMore);

Spacing::Spacing()
{}

TPNode
Spacing::_accept(Consumer& consumer) const {
    return __SPACING.accept(consumer);
}

/*static*/ const Spacing& Spacing::THE_ONE = Spacing();

TPNode
EndOfFile::_accept(Consumer& consumer) const {
    return (consumer.isEOF()) ? toNode(new Token("<EOF>", consumer)) : nullptr;
}

/*static*/ const EndOfFile& EndOfFile::THE_ONE = EndOfFile();

TPNode
Quoted::_accept(Consumer& consumer) const {
    if (consumer[0] != '\'' && consumer[0] != '"')
        return nullptr;
    const char opening = consumer[0];
    size_t n = 1;
    for (; !(consumer[n] == opening) && !consumer.isEOF(n); ++n) {
        if (consumer[n] == '\\') ++n;
    }
    INVARIANT(!consumer.isEOF(n));
    return toNode(consumer.accept(n+1));
}

/*static*/ const Quoted& Quoted::THE_ONE = Quoted();

}
}
