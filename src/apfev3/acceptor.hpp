//
//  acceptor.hpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_acceptor_hpp
#define apfev3_acceptor_hpp

#include <forward_list>
#include "apfev3/consumer.hpp"

namespace apfev3 {
typedef Consumer::Token Token;
class Tokens;
typedef std::forward_list<const Tokens*> ListOfTokens;

class Tokens {
public:
    const enum EType {eTerminal, eSequence, eAlternatives, eEmpty} type;
    
    explicit Tokens()
    : type(eEmpty)
    {}
    
    // Simple constructor for single element.
    explicit Tokens(const Token* ele);
    
    explicit Tokens(const ListOfTokens* from);
    
    explicit Tokens(const Tokens* from);
    
    // Valid only for eAlternatives.
    explicit Tokens(EType type);
    
    Tokens* addAlternative(const Tokens* alt);
    
    Tokens* operator<<(const Tokens* alt) {
        return addAlternative(alt);
    }
    
private:
    //not allowed
    const Tokens& operator=(const Tokens&);
    
    virtual ~Tokens();
    
    union U {
        const Token* terminal;
        const ListOfTokens* sequence;
        const ListOfTokens* alternatives;
        ~U(){}
        U(){}
    } __items;
};

class _Acceptor {
public:
    virtual ~_Acceptor() = 0;
    
    //allow default copy constructors
    
    virtual const Tokens* accept(Consumer& consumer) const = 0;
};

class Terminal : public _Acceptor {
public:
    explicit Terminal(const std::string& text)
    : __text(text)
    {}
    
    virtual const Tokens* accept(Consumer& consumer) const;
    
    virtual ~Terminal(){}
    
private:
    //Not allowed
    Terminal(const Terminal&);
    
    const Terminal& operator=(const Terminal&);
    
    const std::string&  __text;
};

class Repetition : public _Acceptor {
public:
    const enum EType {eOptional, eZeroOrMore, eOneOrMore} type;
    
    explicit Repetition(_Acceptor& ele, EType _type)
    : __ele(ele), type(_type) {}
    
    virtual const Tokens* accept(Consumer& consumer) const;
    
private:
    _Acceptor& __ele;
};

class Sequence : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Sequence(const std::forward_list<std::reference_wrapper<_Acceptor>>& eles)
    : __eles(eles)
    {}
    
    virtual const Tokens* accept(Consumer& consumer) const;

    virtual ~Sequence();
    
private:
    const std::forward_list<std::reference_wrapper<_Acceptor>> __eles;
};

class Alternatives : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Alternatives(const std::forward_list<std::reference_wrapper<_Acceptor>>& eles)
    : __eles(eles)
    {}
    
    virtual const Tokens* accept(Consumer& consumer) const;

    virtual ~Alternatives();
    
private:
    const std::forward_list<std::reference_wrapper<_Acceptor>> __eles;
};
}
#endif /* apfev3_acceptor_hpp */
