//
//  acceptor.hxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#ifndef apfev3_acceptor_hxx
#define apfev3_acceptor_hxx

#include <vector>
#include <regex>
#include "xyzzy/refcnt.hxx"
#include "apfev3/util.hxx"
#include "apfev3/node.hxx"
#include "apfev3/consumer.hxx"


namespace apfev3 {

class _Acceptor {
public:
    virtual ~_Acceptor() = 0;
    
    //allow default copy constructors
    
    virtual TPNode accept(Consumer& consumer) const;
        
protected:
    virtual TPNode _accept(Consumer& consumer) const = 0;
    
    // Subclass which checks for EOF should return false.
    virtual bool _checksForEOF() const {
        return false;
    }
    
private:
    TPNode __accept(Consumer& consumer) const;
};

class Regex : public _Acceptor {
public:
    explicit Regex(const std::string& pattern);
    
     //allow default copy constructors
    
    virtual ~Regex(){}

protected:
    virtual TPNode _accept(Consumer& consumer) const;
    
    virtual TPToken _skipTrailingWs(Consumer& consumer, const std::string& text) const;
     
    virtual TPToken _create(Consumer& consumer, const std::string& text) const;
    
protected:
    const std::regex _rex;
};

class Repetition : public _Acceptor {
public:
    const enum EType {eOptional, eZeroOrMore, eOneOrMore} type;
    
    explicit Repetition(const _Acceptor& ele, EType _type)
    : __ele(ele), type(_type) {}
    
    //allow default copy constructors
    
protected:
    virtual TPNode _accept(Consumer& consumer) const;
    
private:
    const _Acceptor& __ele;
};

typedef SList<const _Acceptor*> AcceptorList;

class Sequence : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Sequence(const std::initializer_list<const _Acceptor*>& eles)
    : __eles(eles)
    {}
    
    //allow default copy constructors
    
    virtual ~Sequence();

protected:
    virtual TPNode _accept(Consumer& consumer) const;

private:
    const AcceptorList __eles;
};

class Alternatives : public _Acceptor {
public:
    // Constructor is {_Acceptor&...}
    explicit Alternatives(const std::initializer_list<const _Acceptor*>& eles)
    : __eles(eles)
    {}
    
    //allow default copy constructors
    
    virtual ~Alternatives();
    
protected:
    virtual TPNode _accept(Consumer& consumer) const;
    
private:
    const AcceptorList __eles;
};

// Some useful tokens
namespace token {

class Ident : public Regex {
public:
    // Use default pattern
    explicit Ident();
    
    explicit Ident(const std::string& patt);
    
    //allow default copy constructors
    
    virtual ~Ident()
    {}
    
    static const Ident& THE_ONE;
    
protected:
    virtual TPToken _create(Consumer& consumer, const std::string& text) const;
};

class LineComment : public _Acceptor {
public:
    explicit LineComment()
    {}
    
    //allow default copy constructors
    
    virtual ~LineComment()
    {}
    
    static const LineComment& THE_ONE;
    
protected:
    virtual TPNode _accept(Consumer& consumer) const;
};

class BlockComment : public _Acceptor {
public:
    explicit BlockComment()
    {}
    
    //allow default copy constructors
    
    virtual ~BlockComment()
    {}
    
    static const BlockComment& THE_ONE;
    
protected:
    virtual TPNode _accept(Consumer& consumer) const;
};

class WhiteSpace : public Regex {
public:
    // Use default pattern
    explicit WhiteSpace();
    
    explicit WhiteSpace(const std::string& patt);
    
    //allow default copy constructors
    
    virtual ~WhiteSpace()
    {}
    
    static const WhiteSpace& THE_ONE;
};


class Spacing : public _Acceptor {
public:
    // Use default pattern
    explicit Spacing();
    
    //allow default copy constructors
    
    virtual ~Spacing()
    {}
    
    static const Spacing& THE_ONE;

protected:
    virtual TPNode _accept(Consumer& consumer) const;
};

class EndOfFile : public _Acceptor {
public:
    // Use default pattern
    explicit EndOfFile()
    {}
    
    //allow default copy constructors
    
    virtual ~EndOfFile()
    {}
    
    static const EndOfFile& THE_ONE;

protected:
    virtual TPNode _accept(Consumer& consumer) const;
    
    virtual bool _checksForEOF() const {
        return true;
    }
};

class Quoted : public _Acceptor {
public:
    //Accepts single or double quoted
    explicit Quoted()
    {}
    
    //allow default copy constructors
    
    virtual ~Quoted()
    {}
    
    static const Quoted& THE_ONE;
    
protected:
    virtual TPNode _accept(Consumer& consumer) const;
};

}   //namespace token
}   //namespace apfev3
#endif /* apfev3_acceptor_hxx */
