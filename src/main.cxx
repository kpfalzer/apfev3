//
//  main.cxx
//  apfev3
//
//  Created by Karl W Pfalzer on 11/20/19.
//  Copyright © 2019 Karl W Pfalzer. All rights reserved.
//

#include <iostream>
#include "apfev3/acceptor.hxx"
#include "apfev3/reduce.hxx"

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
        {
            apfev3::TPTokenVectors min = apfev3::reduce(match);
            size_t n = min->size();
            INVARIANT(1 == n);
            n = min->at(0)->size();
            INVARIANT(1 == n);
            std::cout << min->at(0);
        }
        INVARIANT(!match.isNull());
        match = apfev3::token::EndOfFile::THE_ONE.accept(consumer);
        INVARIANT(match.isValid());
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
        //INVARIANT(consumer.isEOF());
    }
    if (true) {
        const char* p = "abc_def 123 \n456 _123abc\n";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        
        static apfev3::Alternatives TOKEN({&NUMBER,&apfev3::token::Ident::THE_ONE});
        static apfev3::Repetition TOKENS(TOKEN, apfev3::Repetition::eOneOrMore);
        apfev3::TPTokens match = TOKENS.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
        
        std::cout << "match=" << match << std::endl;
    }
    if (true) {
        const char* p =
        "abc_def \n"
        "// this is a line comment\n"
        "//and this is one too\n"
        "/*block\n"
        "\n"
        "//*/IDENT  and_ident1111";
        apfev3::CharBuf cbuf(p);
        apfev3::Consumer consumer(cbuf);
        
        static apfev3::Alternatives TOKEN({
            &apfev3::token::Ident::THE_ONE,
            &apfev3::token::LineComment::THE_ONE,
            &apfev3::token::BlockComment::THE_ONE
        });
        static apfev3::Repetition TOKENS(TOKEN, apfev3::Repetition::eOneOrMore);
        apfev3::TPTokens match = TOKENS.accept(consumer);
        INVARIANT(!match.isNull());
        INVARIANT(consumer.isEOF());
        std::cout << "match=" << match << std::endl;
        {
            apfev3::TPTokenVectors min = apfev3::reduce(match);
            size_t n = min->size();
            INVARIANT(1 == n);
            n = min->at(0)->size();
            INVARIANT(6 == n);
            std::cout << min->at(0);
        }
    }
   if (true) {
       const char* p =
       "1234\n"
       "// this is a line comment\n"
       "//and this is one too\n"
       "/*block\n"
       "\n"
       "//*/5678 \n"
       "10123     ";
       apfev3::CharBuf cbuf(p);
       apfev3::Consumer consumer(cbuf);
       static apfev3::Regex INTEGER("\\d+");
       static apfev3::Alternatives TOKEN({
           &INTEGER,
           &apfev3::token::Spacing::THE_ONE
       });
       static apfev3::Repetition TOKENS(TOKEN, apfev3::Repetition::eOneOrMore);
       static apfev3::Sequence PRODUCTION({&TOKENS, &apfev3::token::EndOfFile::THE_ONE});
       apfev3::TPTokens match = PRODUCTION.accept(consumer);
       INVARIANT(!match.isNull());
       INVARIANT(consumer.isEOF());
       std::cout << "match=" << match << std::endl;
       {
           apfev3::TPTokenVectors min = apfev3::reduce(match);
           size_t n = min->size();
           //INVARIANT(1 == n);
           n = min->at(0)->size();
           //INVARIANT(6 == n);
           std::cout << min->at(0);
       }

   }
    
    std::cout << "Hello world" << std::endl;
    return(0);
}
