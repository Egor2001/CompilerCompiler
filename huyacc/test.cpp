#include <iostream>

#include "SLexem.h"
#include "CBuilder.h"
#include "CHuyacc.h"

int main()
{
    auto lex_term_l = std::make_shared<SLexem>(SLexTerm("left"));
    auto lex_term_r = std::make_shared<SLexem>(SLexTerm("right"));
    auto lex_cat = std::make_shared<SLexem>(SLexCat(lex_term_l, lex_term_r));

    //lex_cat->build_parse(std::cout);

    CBuilder builder;
    auto lex = builder.parse("(<imm>|<lit>|<lol>){<op>(<imm>|<lit>)}");

    if (lex)
        lex->build_parse(std::cout);

    return 0;
}
