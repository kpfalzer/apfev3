//
//  peg2cxx.hxx
//  
//  Created by Karl W Pfalzer on 12/11/19.
//

#ifndef peg2cxx_peg2cxx_hxx
#define peg2cxx_peg2cxx_hxx

#include <iostream>
#include "xyzzy/refcnt.hxx"
#include "apfev3/acceptor.hxx"

namespace peg2cxx {

using xyzzy::PTRcObjPtr;
using xyzzy::downcast;

using std::ostream;
using std::endl;
using apfev3::TPNode;
using apfev3::NodeVector;
using apfev3::_Terminal;
using apfev3::_NonTerminal;
using apfev3::_Acceptor;
using apfev3::Sequence;
using apfev3::Repetition;
using apfev3::Alternatives;
using apfev3::Consumer;
using apfev3::Location;
using apfev3::Mark;
using apfev3::toTerminal;
using apfev3::toNonTerminal;
using apfev3::toNodeVector;
using apfev3::getTypeCode;
using apfev3::AlternativeNode;

#define DEF_TO_XXXNODE(_type)                                       \
inline TP##_type##Node to##_type##Node(const TPNode& node) {        \
        return xyzzy::downcast<apfev3::_Node, _type::Node>(node);   \
    }

//For inner class
#define DEF_TO_XXXICNODE(_type, _name)                              \
inline TP##_name##Node to##_name##Node(const TPNode& node) {        \
        return xyzzy::downcast<apfev3::_Node, _type::Node>(node);   \
    }

#define NODE_TYPE_DECLARE                   \
    virtual std::size_t typeCode() const {  \
        return TYPE_CODE;                   \
    }                                       \
    static const std::size_t TYPE_CODE

#define WITH_NODE_DEFINE(_cls)                                          \
    const _cls& _cls::THE_ONE = _cls();                                 \
    const std::size_t _cls::Node::TYPE_CODE = getTypeCode<_cls::Node>()

}

#endif /* peg2cxx_peg2cxx_hxx */
