//
//  @CLASS@.hxx
//  
//
//  Created by Karl W Pfalzer.
//

#ifndef peg2cxx_@CLASS@_hxx
#define peg2cxx_@CLASS@_hxx

#include "peg2cxx/peg2cxx.hxx"

namespace peg2cxx {

class @CLASS@ : public _Acceptor {
public:
    explicit @CLASS@()
    {}
    
    virtual ~@CLASS@()
    {}
    
    class Node : public NodeVector {
    public:
        virtual ~Node()
        {}
        
        virtual ostream& operator<<(ostream& os) const;
        
		NODE_TYPE_DECLARE;

    private:
        friend class @CLASS@;

		explicit Node(const TPNode& node);
    };
    
    static const @CLASS@& THE_ONE;
    
protected:
    TPNode _accept(Consumer& consumer) const;
};

typedef PTRcObjPtr<@CLASS@::Node> TP@CLASS@Node;
DEF_TO_XXXNODE(@CLASS@)

}

#endif /* peg2cxx_@CLASS@_hxx */
