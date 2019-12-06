//
//  reduce.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 12/4/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include "apfev3/reduce.hxx"

namespace apfev3 {

//fwd reference
#if defined(NOTUSED)
static TPTokenVectors reduce(TPTokenVectors solns, const TPTokens& start);
#endif //NOTUSED

static TPTokenVectors reduce(TPTokenVector   soln, const TPTokens& start);

static void add(TPTokenVectors& allSolns, TPTokenVectors& solns) {
    if (solns.isNull()) return;
    if (allSolns.isNull()) {
        allSolns = new TokenVectors();
    }
    for (auto iter = solns->begin(); iter != solns->end(); ++iter) {
        if (iter->isValid())
            allSolns->push_back(*iter);
    }
    if (allSolns->empty())
        allSolns = nullptr;
}

static void add(TPTokenVectors& allSolns, TPTokenVector& soln) {
    if (soln.isNull()) return;
    if (allSolns.isNull()) {
        allSolns = new TokenVectors();
    }
    allSolns->push_back(soln);
}

static TPTokenVector tryToken(TPTokenVector soln, const TPToken& terminal) {
    if (terminal.isNull())
        return nullptr;
    if (!soln->empty()) {
        const TPToken& last = soln->back();
        if (last.isNull() || (terminal->location <= last->location))
            return nullptr;
    }
    soln->push_back(new Consumer::Token(*terminal));
    return soln;
}

#if defined(NOTUSED)
static TPTokenVectors tryToken(TPTokenVectors solns, const TPToken& terminal) {
    TPTokenVectors allSolns;
    while (solns.isValid() && !solns->empty()) {
        TPTokenVector soln = solns->back();
        solns->pop_back();
        soln = tryToken(soln, terminal);
        add(allSolns, soln);
    }
    return allSolns;
}
#endif //NOTUSED

static TPTokenVectors trySequence(TPTokenVector soln, const TPTokensList& seq) {
    TPTokenVectors allSolns, solns;
    add(solns, soln);
    for (auto iter = seq->iterator(); iter.hasMore(); ) {
        const TPTokens& ele = iter.next();
        TPTokenVectors iterSolns;
        while (solns.isValid() && !solns->empty()) {
            soln = solns->back();
            solns->pop_back();
            TPTokenVectors building = reduce(soln, ele);
            add(iterSolns, building);
        }
        solns = iterSolns;
    }
    add(allSolns, solns);
    return allSolns;
}

#if defined(NOTUSED)
static TPTokenVectors trySequence(TPTokenVectors solns, const TPTokensList& seq) {
    TPTokenVectors allSolns;
    while (solns.isValid() && !solns->empty()) {
        TPTokenVector soln = solns->back();
        solns->pop_back();
        TPTokenVectors solns = trySequence(soln, seq);
        add(allSolns, solns);
    }
    return allSolns;
}
#endif //NOTUSED

static TPTokenVectors tryAlternatives(TPTokenVector soln, const TPTokensList& alts) {
    TPTokenVectors allSolns;
    if (alts.isNull())
        return allSolns;
    for (auto iter = alts->iterator(); iter.hasMore(); ) {
        const TPTokens& ele = iter.next();
        //make sure to dup the soln as we split
        TPTokenVectors solns = reduce(new TokenVector(*soln), ele);
        add(allSolns, solns);
    }
    return allSolns;
}

#if defined(NOTUSED)
static TPTokenVectors tryAlternatives(TPTokenVectors solns, const TPTokensList& alts) {
    TPTokenVectors allSolns;
    while (solns.isValid() && !solns->empty()) {
        TPTokenVector soln = solns->back();
        solns->pop_back();
        TPTokenVectors solns = tryAlternatives(soln, alts);
        add(allSolns, solns);
    }
    return allSolns;
}
#endif //NOTUSED

static TPTokenVectors reduce(TPTokenVector   soln, const TPTokens& start) {
    TPTokenVectors solns;
    switch (start->type) {
        case Tokens::eSequence:
            solns = trySequence(soln, start->asSequence());
            break;
        case Tokens::eTerminal:
            soln = tryToken(soln, start->asToken());
            if (soln.isValid())
                add(solns, soln);
            break;
        case Tokens::eEmpty:
            add(solns, soln);
            break;
        case Tokens::eAlternatives:
            solns = tryAlternatives(soln, start->asAlternatives());
            break;
        default:
            INVARIANT(false);
    }
    return solns;
}

TPTokenVectors reduce(const TPTokens& start) {
    TPTokenVector soln = new TokenVector();
    return reduce(soln, start);
}

std::ostream&
operator<<(std::ostream& os, const TokenVector& items) {
    for (auto iter = items.cbegin(); iter != items.cend(); ++iter) {
        os << *iter << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenVectors& items) {
    size_t i = 1;
    for (auto iter = items.cbegin(); iter != items.cend(); ++iter) {
        os << "Solution " << i << std::endl
        << "================" << std::endl;
        os << *iter << std::endl;
    }
    return os;
}

}
