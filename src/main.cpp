//
//  main.cpp
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <iostream>
#include "apfev3/acceptor.hpp"

int main(int argc, const char * argv[]) {
    {
        const std::string fname = __FILE__;
        apfev3::CharBuf cbuf(fname);
    }
    {
        apfev3::SList<int> list({1,2});
        list << 3 << 4;
        int sum = 0;
        for (auto iter = list.iterator(); iter.hasMore();) {
            sum += iter.next();
        }
        std::cout << "sum=" << sum << std::endl;
    }
    {
        const char* p = "12345";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        apfev3::Regex rex("\\d+");
        apfev3::TPTokens match = rex.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
    }
    static apfev3::Regex NUMBER("\\d+\\s*");
    static apfev3::Regex IDENT("[_a-zA-Z][_a-zA-Z\\d]*\\s*");
    {
        const char* const p = "abc_def \n";
        bool m = std::regex_match(p, std::regex("[_a-zA-Z][_a-zA-Z\\d]*\\s*"));
        INVARIANT(m);
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        apfev3::TPTokens match = IDENT.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
    }
    if (true) {
        const char* p = "abc_def \n\n";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        
        static apfev3::Alternatives TOKEN({&NUMBER,&IDENT});
        apfev3::TPTokens match = TOKEN.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
    }
    if (true) {
        const char* p = "abc_def 123 \n456 _123abc\n";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);    
        
        static apfev3::Alternatives TOKEN({&NUMBER,&IDENT});
        static apfev3::Repetition TOKENS(TOKEN, apfev3::Repetition::eOneOrMore);
        apfev3::TPTokens match = TOKENS.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
        
        std::cout << "match=" << match << std::endl;
    }
    std::cout << "Hello world" << std::endl;
    return(0);
}
