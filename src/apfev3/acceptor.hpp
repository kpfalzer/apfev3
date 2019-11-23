//
//  acceptor.hpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_acceptor_hpp
#define apfev3_acceptor_hpp

#include <regex>
#include "apfev3/consumer.hpp"

namespace apfev3 {
typedef Consumer::Token Token;
typedef SingleOwnerPtr<Token> TPToken;
class Tokens;
typedef SingleOwnerPtr<Tokens> TPTokens;

typedef SList<const TPTokens> ListOfTokens;
typedef SingleOwnerPtr<ListOfTokens>   TPListOfTokens;

class Tokens {
public:
    const enum EType {eTerminal, eSequence, eAlternatives, eEmpty} type;
    
    explicit Tokens()
    : type(eEmpty)
    {}
    
    // Simple constructor for single element.
    explicit Tokens(const TPToken& ele);
    
    explicit Tokens(const TPListOfTokens& from);
    
    explicit Tokens(const TPTokens& from);
    
    // Valid only for eAlternatives.
    explicit Tokens(EType type);
    
    void addAlternative(const TPTokens& alt);
        
    virtual ~Tokens();
    
private:
    //not allowed
    const Tokens& operator=(const Tokens&);
    
    Tokens(const Tokens&);
    
    union U {
        TPToken terminal;
        TPListOfTokens sequence;
        TPListOfTokens alternatives;
        ~U(){}
        U(){}
    } __items;
};

class _Acceptor {
public:
    virtual ~_Acceptor() = 0;
    
    //allow default copy constructors
    
    virtual const TPTokens accept(Consumer& consumer) const;
    
protected:
    virtual const TPTokens _accept(Consumer& consumer) const = 0;
};

class Terminal : public _Acceptor {
public:
    explicit Terminal(const std::string& text)
    : __text(text)
    {}
    
    //allow default copy constructors
    
    virtual ~Terminal(){}

protected:
    virtual const TPTokens _accept(Consumer& consumer) const;
    
private:
    const std::string&  __text;
};

class Regex : public _Acceptor {
public:
    explicit Regex(const std::string& pattern);
    
     //allow default copy constructors
    
    virtual ~Regex(){}

protected:
    virtual const TPTokens _accept(Consumer& consumer) const;
     
private:
    std::regex __rex;
};

class Repetition : public _Acceptor {
public:
    const enum EType {eOptional, eZeroOrMore, eOneOrMore} type;
    
    explicit Repetition(const _Acceptor& ele, EType _type)
    : __ele(ele), type(_type) {}
    
    //allow default copy constructors
    
protected:
    virtual const TPTokens _accept(Consumer& consumer) const;
    
private:
    const _Acceptor& __ele;
};

typedef SList<const _Acceptor*> TListOfAcceptor;

class Sequence : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Sequence(const TListOfAcceptor& eles)
    : __eles(eles)
    {}
    
    //allow default copy constructors
    
    virtual ~Sequence();

protected:
    virtual const TPTokens _accept(Consumer& consumer) const;

private:
    const TListOfAcceptor& __eles;
};

class Alternatives : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Alternatives(const TListOfAcceptor& eles)
    : __eles(eles)
    {}
    
    //allow default copy constructors
    
    virtual ~Alternatives();
    
protected:
    virtual const TPTokens _accept(Consumer& consumer) const;
    
private:
    const TListOfAcceptor& __eles;
};
}
#endif /* apfev3_acceptor_hpp */
