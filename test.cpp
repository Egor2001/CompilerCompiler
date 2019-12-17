#include <iostream>
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

    CParser parser;
    CFlexx flexx;
    CHuyacc huyacc;

    const auto& lexem_vec_ref = parser.parse(flexx_text);
    parser.print(std::cout);
    flexx.flexx(lexem_vec_ref, std::cout);
    huyacc.huyacc(lexem_vec_ref, std::cout);

    return 0;
}
