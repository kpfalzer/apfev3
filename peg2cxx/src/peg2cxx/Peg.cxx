//
//  Peg.cxx
//  
//
//  Created by Karl W Pfalzer
//

#include "peg2cxx/Peg.hxx"

namespace peg2cxx {

TPNode
Peg::_accept(Consumer& consumer) const {
	//todo
	TPNode node = nullptr; //todo
    return (node.isValid()) ? new Node(node) : nullptr;
}

WITH_NODE_DEFINE(Peg);

Peg::Node::Node(const TPNode& node)
: NodeVector(node)
{}

ostream&
Peg::Node::operator<<(ostream& os) const {
    return NodeVector::operator<<(os);
}

}
