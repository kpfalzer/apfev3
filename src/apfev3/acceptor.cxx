//
//  acceptor.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "apfev3/util.hxx"
#include "apfev3/acceptor.hxx"

namespace apfev3 {

Tokens::Tokens(const TPToken& ele)
: type(eTerminal){
    __items.terminal = ele;
}

Tokens::Tokens(const TPTokensList& from, EType type)
:type(type) {
    switch(type)  {
        case eSequence:
            __items.sequence = from;
            break;
        case eAlternatives:
            __items.alternatives = from;
            break;
        default:
            INVARIANT(false);
    }
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
        __items.alternatives = new TokensList();
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

static std::ostream& printAlternatives(std::ostream& os, const TPTokensList& eles) {
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
    // Grab alternates before
    TPConsumerList alts = (consumer.hasAlts()) ? consumer.alts() : nullptr;
    TPTokens tokens = __accept(consumer);
    if (alts.isValid()) {
        TPConsumerList alive;
        TPTokensList altTokens = new TokensList();
        if (nullptr != tokens) altTokens->append(tokens);
        for (auto iter = alts->iterator(); iter.hasMore();) {
            Consumer& xconsumer = iter.next();
            TPTokens alt = accept(xconsumer);
            if (nullptr != alt) {
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

const TPTokens _Acceptor::__accept(Consumer& consumer) const {
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
    TokensList* tokens = nullptr;
    while (true) {
        p = __ele.accept(consumer);
        if (nullptr == p) break;
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

const TPTokens Sequence::_accept(Consumer& consumer) const {
    TPTokens p;
    TokensList* tokens = nullptr;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        p = iter.next()->accept(consumer);
        if (nullptr == p) return nullptr;
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

const TPTokens Alternatives::_accept(Consumer& consumer) const {
    TPTokens p;
    Tokens* alts = nullptr;
    const Consumer start(consumer);
    bool updateBase = true;
    for (auto iter = __eles.iterator(); iter.hasMore(); ) {
        Consumer xconsumer(start);
        p = iter.next()->accept(xconsumer);
        if (nullptr != p) {
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

static const std::string IDENT_PATT = "([_a-zA-Z][_a-zA-Z\\d]*)\\s*";
Ident::Ident() : Ident(IDENT_PATT) {}
Ident::Ident(const std::string& patt) : Regex(patt) {}
Ident::~Ident() {}
TPToken Ident::_create(Consumer& consumer, const std::string& text) const {
    return _skipTrailingWs(consumer, text);
}

/*static*/ const Ident& Ident::THE_ONE = Ident();
 
const TPTokens
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

const TPTokens
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

}
}
