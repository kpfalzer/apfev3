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

static TPTokenVectors __reduce(TPTokenVector   soln, const TPTokens& start);

static void __add(TPTokenVectors& allSolns, TPTokenVectors& solns) {
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

static void __add(TPTokenVectors& allSolns, TPTokenVector& soln) {
    if (soln.isNull()) return;
    if (allSolns.isNull()) {
        allSolns = new TokenVectors();
    }
    allSolns->push_back(soln);
}

static TPTokenVector __tryToken(TPTokenVector soln, const TPToken& terminal) {
    if (terminal.isNull())
        return nullptr;
    if (!soln->empty()) {
        const TPToken& last = soln->back();
        if (last.isNull() || (terminal->location <= last->location))
            return nullptr;
    }
    soln->push_back(new Consumer::Token(terminal.asT()));
    return soln;
}

static TPTokenVectors __trySequence(TPTokenVector soln, const TPTokensList& seq) {
    TPTokenVectors allSolns, solns;
    __add(solns, soln);
    for (auto iter = seq->iterator(); iter.hasMore(); ) {
        const TPTokens& ele = iter.next();
        TPTokenVectors iterSolns;
        while (solns.isValid() && !solns->empty()) {
            soln = solns->back();
            solns->pop_back();
            TPTokenVectors building = __reduce(soln, ele);
            __add(iterSolns, building);
        }
        solns = iterSolns;
    }
    __add(allSolns, solns);
    return allSolns;
}

static TPTokenVectors __tryAlternatives(TPTokenVector soln, const TPTokensList& alts) {
    TPTokenVectors allSolns;
    if (alts.isNull())
        return allSolns;
    for (auto iter = alts->iterator(); iter.hasMore(); ) {
        const TPTokens& ele = iter.next();
        //make sure to dup the soln as we split
        TPTokenVectors solns = __reduce(new TokenVector(soln.asT()), ele);
        __add(allSolns, solns);
    }
    return allSolns;
}

static TPTokenVectors __reduce(TPTokenVector   soln, const TPTokens& start) {
    TPTokenVectors solns;
    switch (start->type) {
        case Tokens::eSequence:
            solns = __trySequence(soln, start->asSequence());
            break;
        case Tokens::eTerminal:
            soln = __tryToken(soln, start->asToken());
            if (soln.isValid())
                __add(solns, soln);
            break;
        case Tokens::eEmpty:
            __add(solns, soln);
            break;
        case Tokens::eAlternatives:
            solns = __tryAlternatives(soln, start->asAlternatives());
            break;
        default:
            INVARIANT(false);
    }
    return solns;
}

static TPTokenVectors __reduce(const TPTokens& start) {
    TPTokenVector soln = new TokenVector();
    return __reduce(soln, start);
}

TPTokenVector reduce(const TPTokens& start) {
    TPTokenVectors all = __reduce(start);
    //keep longest
    size_t longest = 0;
    TPTokenVectors best = new TokenVectors();
    for (auto iter = all->cbegin(); iter != all->cend(); ++iter) {
        auto ele = *iter;
        const size_t length = ele->size();
        if (length < longest)
            continue;
        if (length > longest)
            best = new TokenVectors();
        best->push_back(ele);
        longest = length;
    }
    return best->at(0);
}

std::ostream&
operator<<(std::ostream& os, const TokenVector& items) {
    for (auto iter = items.cbegin(); iter != items.cend(); ++iter) {
        os << iter->asT() << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenVectors& items) {
    size_t i = 1;
    for (auto iter = items.cbegin(); iter != items.cend(); ++iter, ++i) {
        os << "Solution " << i << std::endl
        << "================" << std::endl;
        os << iter->asT() << std::endl;
    }
    return os;
}

}
