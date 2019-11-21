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

Tokens::Tokens(const Token* ele)
: type(eTerminal){
    __items.terminal = ele;
}

Tokens::Tokens(const ListOfTokens* from)
:type(eSequence) {
    __items.sequence = from;
}

Tokens::Tokens(const Tokens* from)
: type(from->type), __items(from->__items){
}

// Valid only for eAlternatives.
Tokens::Tokens(EType _type)
: type(_type) {
    INVARIANT(eAlternatives == type);
    __items.alternatives = nullptr;
}

Tokens* Tokens::addAlternative(const Tokens* alt) {
    INVARIANT(eAlternatives == type);
    if (nullptr == __items.alternatives) {
        __items.alternatives = new ListOfTokens();
    }
    ListOfTokens* p = const_cast<ListOfTokens*>(__items.alternatives);
    p->insert_after(p->end(), alt);
    return this;
}

Tokens::~Tokens(){}

_Acceptor::~_Acceptor() {}

const Tokens* Terminal::accept(Consumer& consumer) const {
    const size_t n = __text.length();
    for (size_t i = 0; i < n; i++) {
        if (__text[i] != consumer[i]) {
            return nullptr;
        }
    }
    return new Tokens(consumer.accept(n));
}

const Tokens* Repetition::accept(Consumer& consumer) const {
    const Tokens* p;
    ListOfTokens* tokens = nullptr;
    while (true) {
        p = __ele.accept(consumer);
        if (nullptr == p) break;
        if (nullptr == tokens) {
            tokens = new ListOfTokens();
        }
        tokens->insert_after(tokens->end(), p);
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

const Tokens* Sequence::accept(Consumer& consumer) const {
    const Tokens* p;
    ListOfTokens* tokens = nullptr;
    for (auto acc = __eles.cbegin(); acc != __eles.cend(); acc++) {
        p = acc->get().accept(consumer);
        if (nullptr == p) return nullptr;
        if (nullptr == tokens) {
            tokens = new ListOfTokens();
        }
        tokens->insert_after(tokens->end(), p);
    }
    INVARIANT(nullptr != tokens);
    return new Tokens(tokens);
}

Sequence::~Sequence()
{}

const Tokens* Alternatives::accept(Consumer& consumer) const {
    const Tokens* p;
    Tokens* alts = nullptr;
    for (auto acc = __eles.cbegin(); acc != __eles.cend(); acc++) {
        p = acc->get().accept(consumer);
        if (nullptr != p) {
            if (nullptr == alts) {
                alts = new Tokens(Tokens::eAlternatives);
            }
            *alts << p;
        }
    }
    return alts;
}

Alternatives::~Alternatives()
{}

}
