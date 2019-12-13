//
//  acceptor.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "apfev3/util.hxx"
#include "apfev3/acceptor.hxx"
#include "apfev3/reduce.hxx"

namespace apfev3 {

Tokens::Tokens(const TPToken& ele)
: type(eTerminal){
    __items.terminal.ref(ele);
}

Tokens::Tokens(const TPTokensList& from, EType type)
:type(type) {
    switch(type)  {
        case eSequence:
            __items.sequence.ref(from);
            break;
        case eAlternatives:
            __items.alternatives.ref(from);
            break;
        default:
            INVARIANT(false);
    }
}

// Valid only for eAlternatives.
Tokens::Tokens(EType _type)
: type(_type) {
    INVARIANT(eAlternatives == type);
    __items.alternatives.ref(new TokensList());
}

Tokens::Tokens(const Tokens& from)
: type(from.type) {
    switch(type)  {
        case eSequence:
            __items.sequence.ref(from.asSequence());
            break;
        case eAlternatives:
            __items.alternatives.ref(from.asAlternatives());
            break;
        case eTerminal:
            __items.terminal.ref(from.asToken());
            break;
        default:
            INVARIANT(false);
    }
}


void
Tokens::addAlternative(const TPTokens& alt) {
    INVARIANT(eAlternatives == type);
    if (__items.alternatives.isNull()) {
        __items.alternatives = new TokensList();
    }
    __items.alternatives->append(alt);
}

Tokens::~Tokens(){
    switch(type) {
        case eAlternatives:
            //__items.alternatives.destroy();
            break;
        case eSequence:
            //__items.sequence.destroy();
            break;
        case eTerminal:
            //__items.terminal.destroy();
            break;
        default:
            ;
    }
}

static std::ostream&
__printAlternatives(std::ostream& os, const TPTokensList& eles) {
    static const char* const SEP = " | ";
    os << _SList::open();
    const char* sep = nullptr;
    for (auto iter = eles->iterator(); iter.hasMore(); ) {
        if (nullptr != sep) os << sep;
        iter.next()->operator<<(os);
        sep = SEP;
    }
    os << _SList::close();
    return os;
}

TPToken
Tokens::asToken() const {
    INVARIANT(type == eTerminal);
    return __items.terminal;
}

TPTokensList
Tokens::asSequence() const {
    INVARIANT(type == eSequence);
    return __items.sequence;
}

TPTokensList
Tokens::asAlternatives() const {
    INVARIANT(type == eAlternatives);
    return __items.alternatives;
}

std::ostream&
Tokens::operator<<(std::ostream& os) const {
    switch(type) {
        case eAlternatives:
            __printAlternatives(os, __items.alternatives);
            break;
        case eSequence:
            os << __items.sequence.asT();
            break;
        case eTerminal:
            os << __items.terminal.asT();
            break;
        case eEmpty:
            break;
        default:
            INVARIANT(false);
    }
    return os;
}

TPTokenVector
Tokens::reduce() const {
    TPTokens start = new Tokens(*this);
    return ::apfev3::reduce(start);
}

TokenVectorNode::TokenVectorNode(const TPTokens& tokens) {
    const TPTokenVector toks = tokens->reduce();
    for (auto iter = toks->cbegin(); iter != toks->cend(); ++iter) {
        push_back(*iter);
    }
}
    
TokenVectorNode::~TokenVectorNode() {
};

TPTokens
_Acceptor::accept(Consumer& consumer) const {
    // Grab alternates before
    TPConsumerList alts = (consumer.hasAlts()) ? consumer.alts() : nullptr;
    TPTokens tokens = __accept(consumer);
    if (alts.isValid()) {
        TPConsumerList alive;
        TPTokensList altTokens = new TokensList();
        if (tokens.isValid()) altTokens->append(tokens);
        for (auto iter = alts->iterator(); iter.hasMore();) {
            Consumer& xconsumer = iter.next();
            TPTokens alt = accept(xconsumer);
            if (alt.isValid()) {
                altTokens->append(alt);
                if (consumer != xconsumer) {
                    append(alive, xconsumer);
                }
            }
        }
        consumer.replaceAlts(alive);
        if (! altTokens->isEmpty()) {
            tokens = new Tokens(altTokens, Tokens::eAlternatives);
        }
    }
    return tokens;
}

TPTokens
_Acceptor::__accept(Consumer& consumer) const {
    return (!_checksForEOF() && consumer.isEOF()) ? nullptr : _accept(consumer);
}

TPNode
_Acceptor::toNode(const TPTokens& tokens) const {
    TPTokenVectorNode node(new TokenVectorNode(tokens));
    return xyzzy::upcast<_Node>(node);
}

_Acceptor::~_Acceptor() {}

Regex::Regex(const std::string& pattern)
: _rex(pattern.c_str()) {}
 
TPTokens
Regex::_accept(Consumer& consumer) const {
    std::string s = "";
    size_t n = 0;
    for (; n < consumer.rem(); n++) {
        s += consumer[n];
        if (! std::regex_match(s, _rex)) {
            break;
        }
    }
    if (0 < n) {
        TPToken token = _create(consumer, s.substr(0,n));
        return new Tokens(token);
    } else {
        return nullptr;
    }
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

TPTokens
Repetition::_accept(Consumer& consumer) const {
    TPTokens p;
    TokensList* tokens = nullptr;
    while (true) {
        p = __ele.accept(consumer);
        if (p.isNull()) break;
        if (nullptr == tokens) {
            tokens = new TokensList();
        }
        tokens->append(p);
    }
    if (nullptr == tokens) {
        switch (type) {
            case eOptional:
            case eZeroOrMore:
                return new Tokens();
            default:
                ;//do nothing
        }
    }
    return (nullptr != tokens) ? new Tokens(tokens) : nullptr;
}

TPTokens
Sequence::_accept(Consumer& consumer) const {
    TPTokens p;
    TokensList* tokens = nullptr;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        p = iter.next()->accept(consumer);
        if (p.isNull()) return nullptr;
        if (nullptr == tokens) {
            tokens = new TokensList();
        }
        tokens->append(p);
    }
    INVARIANT(nullptr != tokens);
    return new Tokens(tokens);
}

Sequence::~Sequence()
{}

TPTokens
Alternatives::_accept(Consumer& consumer) const {
    TPTokens p;
    Tokens* alts = nullptr;
    const Consumer start(consumer);
    bool updateBase = true;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        Consumer xconsumer(start);
        p = iter.next()->accept(xconsumer);
        if (p.isValid()) {
            if (nullptr == alts) {
                alts = new Tokens(Tokens::eAlternatives);
            }
            alts->addAlternative(p);
            if (updateBase) {
                //first alt, we update the base consumer
                consumer.rewind(xconsumer);
                updateBase = false;
            } else {
                consumer.addAlt(xconsumer);
            }
        }
    }
    return alts;
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
 
TPTokens
LineComment::_accept(Consumer& consumer) const {
    if (consumer[0] != '/' || consumer[1] != '/')
        return nullptr;
    size_t n = 2;
    for (; consumer[n] != '\n' && !consumer.isEOF(n); ++n)
        ;
    if (consumer.isEOF(n))
        --n;
    return new Tokens(consumer.accept(n+1));
}

/*static*/ const LineComment& LineComment::THE_ONE = LineComment();

TPTokens
BlockComment::_accept(Consumer& consumer) const {
    if (consumer[0] != '/' || consumer[1] != '*')
        return nullptr;
    size_t n = 2;
    for (; !(consumer[n] == '*' && consumer[n+1] == '/') && !consumer.isEOF(n); ++n)
        ;
    INVARIANT(!consumer.isEOF(n));
    return new Tokens(consumer.accept(n+2));
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

TPTokens
Spacing::_accept(Consumer& consumer) const {
    return __SPACING.accept(consumer);
}

/*static*/ const Spacing& Spacing::THE_ONE = Spacing();

TPTokens
EndOfFile::_accept(Consumer& consumer) const {
    return (consumer.isEOF()) ? new Tokens(new Token("<EOF>", consumer)) : nullptr;
}

/*static*/ const EndOfFile& EndOfFile::THE_ONE = EndOfFile();

}
}
