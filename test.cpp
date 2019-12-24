#include <iostream>
#include <fstream>
#include <string>
//#include "CFlexxer.h"
//#include "CBuilder.h"
#include "parser/CParser.h"
#include "flexx/CFlexx.h"
#include "huyacc/CHuyacc.h"

int main()
{
    auto flexx_text = std::string_view(
        "%{\n"
            "<call> call\n"
            "${" "\n"
            "    YYLVAL = std::string(\"[call]\");" "\n"
            "$}" "\n"
            
            "<expr> expr\n"
            "${" "\n"
            "    YYLVAL = std::string(\"[expr]\");" "\n"
            "$}" "\n"
            
            "<block> block\n"
            "${" "\n"
            "    YYLVAL = std::string(\"[block]\");" "\n"
            "$}" "\n"
            
            "<if> if\n"
            "${" "\n"
            "    YYLVAL = std::string(\"[if]\");" "\n"
            "$}" "\n"
/*
            "<imm> imm\n    \n\r\t" 
            "${" "\n"
            "    YYLVAL.name = std::string(\"[imm]\");" "\n"
            "$}" "\n"
*/
            "<if_kwd> if\n    \n\r\t" 
            "${" "\n"
            "    YYLVAL = std::string(\"[if]\");" "\n"
            "$}" "\n"
/*
            "<lit> lit\n\r \t"
            "${ " "\n"
            "    YYLVAL.name = std::string(\"[lit]\");" "\n"
            "$}" "\n"
*/
            "<op> op\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[op]\");" "\n"
            "$}" "\n"

            "<delim> delim\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[delim]\");" "\n"
            "$}" "\n"

            "<brc_beg> brc_beg\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[brc_beg]\");" "\n"
            "$}" "\n"

            "<brc_end> brc_end\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[brc_end]\");" "\n"
            "$}" "\n"

            "<bkt_beg> bkt_beg\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[bkt_beg]\");" "\n"
            "$}" "\n"

            "<bkt_end> bkt_end\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[bkt_end]\");" "\n"
            "$}" "\n"

            "<def> def\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[def]\");" "\n"
            "$}" "\n"

            "<while> while\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[while]\");" "\n"
            "$}" "\n"

            "<test> test\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string(\"[test]\");" "\n"
            "$}" "\n"
        "%}"

        "%{"
            "<test> ::= <lit>\n"
            "${ " "\n"
            "    YYLVAL = YYLIST[2];" "\n"
            "$}" "\n"

            "<call> ::= "
            "    <lit><brc_beg>[<expr>{<delim><expr>}]<brc_end>\n"
            "${ " "\n"
            "    YYLVAL = std::to_string(YYLIST.size());" "\n"
            "$}" "\n"

            "<expr> ::= "
            "    (<imm>|<lit>|<call>)"
            "    [<op><expr>]|<brc_beg><expr><brc_end>\n"
            "${ " "\n"
            "    YYLVAL = std::to_string(YYLIST.size());" "\n"
            "$}" "\n"

            "<block> ::= "
            "    <bkt_beg>{<expr>|<def>|<if>|<while>}<bkt_end>\n"
            "${ " "\n"
//            "    YYLVAL = SBlock(YYLIST);" "\n"
            "$}" "\n"

            "<if> ::= "
            "    <if_kwd><brc_beg><expr><brc_end><block>\n"
            "${ " "\n"
//            "    YYLVAL = SIf(YYLIST);" "\n"
            "$}" "\n"
        "%}"
    );

    auto test_text = std::string_view(
        "%{\n"
        "   <if_> if\n"
        "   ${"
        "       YYLVAL = KeywordToken(KW_IF);"
        "   $}"
        "   <while_> while\n"
        "   ${"
        "       YYLVAL = KeywordToken(KW_WHILE);"
        "   $}"
        /*
            "<test> test\n\r \t"
            "${ " "\n"
            "    YYLVAL = {};" "\n"
            "$}" "\n"
        */
        "%}"

        "%{"
            "<if> ::= <if_><(><expr><)><block>[<else_><block>]\n"
            "${ " "\n"
            "    YYLVAL = YYLIST[0];" "\n"
            "$}" "\n"

            "<test_rule> ::= <lit_rule>|<imm_rule>\n"
            "${ " "\n"
            "    YYLVAL = YYLIST[0];" "\n"
            "$}" "\n"

            "<lit_rule> ::= <lit>\n"
            "${ " "\n"
            "    auto name_str = std::to_string(YYLIST.size());" "\n"
            "    YYLVAL = YYNODE(SFlexxTerm(std::move(name_str)));" "\n"
            "$}" "\n"

            "<imm_rule> ::= <imm>\n"
            "${ " "\n"
            "    auto name_str = std::to_string(YYLIST.size());" "\n"
            "    YYLVAL = YYNODE(SFlexxTerm(std::move(name_str)));" "\n"
            "$}" "\n"
        "%}"
    );

    auto flexx_out = std::ofstream("flexxer.h");
    auto huyacc_out = std::ofstream("huyaccer.h");

    CParser parser;
    CFlexx flexx;
    CHuyacc huyacc;

    //parser.parse(flexx_text);
    parser.parse(test_text);
    parser.print(std::cout);
    flexx.flexx(parser.term_vec(), flexx_out);
    huyacc.huyacc(parser.term_vec(), parser.rule_vec(), huyacc_out);

    return 0;
}
