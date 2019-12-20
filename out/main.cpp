#include <iostream>
#include <string_view>
#include "flexxer.h"
#include "huyaccer.h"

int main()
{
    auto text = std::string_view("aaa 123 lol kek42");
    std::vector<SLexem> lex_vec;
    flexx(text, lex_vec);
    auto node = huyacc(lex_vec);

    if (node)
    {
        if (std::holds_alternative<std::monostate>(node->term.data))
            std::cout << "BIBA\n"; 
        if (std::holds_alternative<std::string>(node->term.data))
            std::cout << "LIT " 
                      << std::get<std::string>(node->term.data) 
                      << "\n"; 
        if (std::holds_alternative<int32_t>(node->term.data))
            std::cout << "IMM " 
                      << std::get<int32_t>(node->term.data) 
                      << "\n"; 
    }

    //TODO: to make visitor, but now I have no time
    for (const auto& lex : lex_vec)
    {
        if (std::holds_alternative<std::string>(lex.term.data))
            std::cout << "[STR " 
                      << std::get<std::string>(lex.term.data) 
                      << "]\n";
        if (std::holds_alternative<int32_t>(lex.term.data))
            std::cout << "[INT " 
                      << std::get<int32_t>(lex.term.data) 
                      << "]\n";
    }

    return 0;
};
