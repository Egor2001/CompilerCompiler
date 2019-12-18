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
/*
        "%{\n"
            "<imm> [1-9]{\\d}\n    \n\r\t" 
            "${" "\n"
            "    std::size_t val = std::from_bytes(YYTEXT);" "\n"
            "    YYLVAL = val;" "\n"
            "$}" "\n"

            "<lit> [_a-zA-Z]{\\w}\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string_view(YYTEXT);" "\n"
            "$}" "\n"

            "<op> \\+|\\-|\\*|\\/|\\%|\\=\n\r \t"
            "${ " "\n"
            "    YYLVAL = std::string_view(YYTEXT).front();" "\n"
            "$}" "\n"
        "%}"
*/
        "%{\n"
            "<imm> imm\n    \n\r\t" 
            "${" "\n"
            "    YYLVAL.name = std::string(\"[imm]\");" "\n"
            "$}" "\n"

            "<if> if\n    \n\r\t" 
            "${" "\n"
            "    YYLVAL.name = std::string(\"[if]\");" "\n"
            "$}" "\n"

            "<lit> lit\n\r \t"
            "${ " "\n"
            "    YYLVAL.name = std::string(\"[lit]\");" "\n"
            "$}" "\n"

            "<op> op\n\r \t"
            "${ " "\n"
            "    YYLVAL.name = std::string(\"[op]\");" "\n"
            "$}" "\n"
        "%}"

        "%{"
            "<call> ::= "
            "    <lit><\"(\">[<expr>{<\",\"><expr>}]<\")\">\n"
            "${ " "\n"
            "    YYLVAL = SCall(YYLIST);" "\n"
            "$}" "\n"

            "<expr> ::= "
            "    (<imm>|<lit>|<call>)"
            "    [<op><expr>]|<\"(\"><expr><\")\">\n"
            "${ " "\n"
            "    YYLVAL = SExpr(YYLIST);" "\n"
            "$}" "\n"

            "<block> ::= "
            "    <\"{\">{<expr>|<def>|<if>|<while>}<\"}\">\n"
            "${ " "\n"
            "    YYLVAL = SBlock(YYLIST);" "\n"
            "$}" "\n"

            "<if> ::= "
            "    <\"if\"><\"(\"><expr><\")\"><block>\n"
            "${ " "\n"
            "    YYLVAL = SIf(YYLIST);" "\n"
            "$}" "\n"
        "%}"
    );

    auto flexx_out = std::ofstream("flexxer.h");
    auto huyacc_out = std::ofstream("huyaccer.h");

    CParser parser;
    CFlexx flexx;
    CHuyacc huyacc;

    parser.parse(flexx_text);
    parser.print(std::cout);
    flexx.flexx(parser.term_vec(), flexx_out);
    huyacc.huyacc(parser.rule_vec(), huyacc_out);

    return 0;
}
