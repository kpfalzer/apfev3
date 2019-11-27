//
//  acceptor.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "apfev3/util.hpp"
#include "apfev3/acceptor.hpp"

namespace apfev3 {

Tokens::Tokens(const TPToken& ele)
: type(eTerminal){
    __items.terminal = ele;
}

Tokens::Tokens(const TPListOfTokens& from)
:type(eSequence) {
    __items.sequence = from;
}

Tokens::Tokens(const TPTokens& from)
: type(from->type){
    //todo: __items
}

// Valid only for eAlternatives.
Tokens::Tokens(EType _type)
: type(_type) {
    INVARIANT(eAlternatives == type);
    __items.alternatives = nullptr;
}

void Tokens::addAlternative(const TPTokens& alt) {
    INVARIANT(eAlternatives == type);
    if (nullptr == __items.alternatives) {
        __items.alternatives = new ListOfTokens();
    }
    __items.alternatives->append(alt);
}

Tokens::~Tokens(){
    switch(type) {
        case eAlternatives:
            __items.alternatives.destroy();
            break;
        case eSequence:
            __items.sequence.destroy();
            break;
        case eTerminal:
            __items.terminal.destroy();
            break;
        default:
            ;
    }
}

static std::ostream& printAlternatives(std::ostream& os, const TPListOfTokens& eles) {
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

std::ostream& Tokens::operator<<(std::ostream& os) const {
    switch(type) {
        case eAlternatives:
            printAlternatives(os, __items.alternatives);
            break;
        case eSequence:
            os << *(__items.sequence);
            break;
        case eTerminal:
            os << *(__items.terminal);
            break;
        case eEmpty:
            break;
        default:
            INVARIANT(false);
    }
    return os;
}

const TPTokens _Acceptor::accept(Consumer& consumer) const {
    return (consumer.isEOF()) ? nullptr : _accept(consumer);
}

_Acceptor::~_Acceptor() {}

const TPTokens Terminal::_accept(Consumer& consumer) const {
    const size_t n = __text.length();
    for (size_t i = 0; i < n; i++) {
        if (__text[i] != consumer[i]) {
            return nullptr;
        }
    }
    return new Tokens(consumer.accept(n));
}

Regex::Regex(const std::string& pattern)
: _rex(pattern.c_str()) {}
 
const TPTokens Regex::_accept(Consumer& consumer) const {
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

TPToken Regex::_create(Consumer& consumer, const std::string& text) const {
    return consumer.accept(text.length());
}

TPToken Regex::_skipTrailingWs(Consumer& consumer, const std::string& text) const {
    std::smatch match;
    INVARIANT(std::regex_match(text, match, _rex));
    const std::string ident = match[1];
    return consumer.accept(text.length(), 0, match[1].length()-1);
}

const TPTokens Repetition::_accept(Consumer& consumer) const {
    TPTokens p;
    ListOfTokens* tokens = nullptr;
    while (true) {
        p = __ele.accept(consumer);
        if (nullptr == p) break;
        if (nullptr == tokens) {
            tokens = new ListOfTokens();
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

const TPTokens Sequence::_accept(Consumer& consumer) const {
    TPTokens p;
    ListOfTokens* tokens = nullptr;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        p = iter.next()->accept(consumer);
        if (nullptr == p) return nullptr;
        if (nullptr == tokens) {
            tokens = new ListOfTokens();
        }
        tokens->append(p);
    }
    INVARIANT(nullptr != tokens);
    return new Tokens(tokens);
}

Sequence::~Sequence()
{}

// Try all alternatives
const TPTokens Alternatives::_accept(Consumer& xconsumer) const {
    TPTokens p;
    Tokens* alts = nullptr;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        Consumer consumer(xconsumer);
        p = iter.next()->accept(consumer);
        if (nullptr != p) {
            if (nullptr == alts) {
                alts = new Tokens(Tokens::eAlternatives);
            }
            alts->addAlternative(p);
        }
    }
    return alts;
}

Alternatives::~Alternatives()
{}

namespace token {

static const std::string IDENT_PATT = "([_a-zA-Z][_a-zA-Z\\d]*)\\s*";
Ident::Ident() : Ident(IDENT_PATT) {}
Ident::Ident(const std::string& patt) : Regex(patt) {}
Ident::~Ident() {}
TPToken Ident::_create(Consumer& consumer, const std::string& text) const {
    return _skipTrailingWs(consumer, text);
}

/*static*/ const Ident& Ident::THE_ONE = Ident();

}
}
