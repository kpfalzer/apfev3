//
//  Peg.hxx
//  
//
//  Created by Karl W Pfalzer.
//

#ifndef peg2cxx_Peg_hxx
#define peg2cxx_Peg_hxx

#include "peg2cxx/peg2cxx.hxx"

namespace peg2cxx {

class Peg : public _Acceptor {
public:
    explicit Peg()
    {}
    
    virtual ~Peg()
    {}
    
    class Node : public NodeVector {
    public:
        virtual ~Node()
        {}
        
        virtual ostream& operator<<(ostream& os) const;
        
		NODE_TYPE_DECLARE;

    private:
        friend class Peg;

		explicit Node(const TPNode& node);
    };
    
    static const Peg& THE_ONE;
    
protected:
    TPNode _accept(Consumer& consumer) const;
};

typedef PTRcObjPtr<Peg::Node> TPPegNode;
DEF_TO_XXXNODE(Peg)

}

#endif /* peg2cxx_Peg_hxx */
