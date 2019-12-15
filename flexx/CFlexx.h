#ifndef CFLEXX_H
#define CFLEXX_H

#include <optional>
#include <string_view>
#include <regex>

#include "../parser/SParseLexem.h"

//namespace huyacc {

struct SToken
{
};

class CFlexx
{
public:
    CFlexx() = default;

    template<typename TStream>
    bool flexx(const std::vector<SParseLexem>& lexem_vec,
               TStream& out_stream);

private:
    template<typename TStream>
    bool flexx_includes(TStream& out_stream);
    template<typename TStream>
    bool flexx_structure(TStream& out_stream);

    template<typename TStream>
    bool flexx_instance(const SParseLexem& lexem, TStream& out_stream);
    template<typename TStream>
    bool flexx_function(const SParseLexem& lexem, TStream& out_stream);

private:
};

template<typename TStream>
bool CFlexx::flexx(const std::vector<SParseLexem>& lexem_vec,
                   TStream& out_stream)
{
    bool ok = true;

    //TODO: to fix it!
    std::size_t term_cnt = 0u;
    for (const auto& lexem : lexem_vec)
    {
        if (std::holds_alternative<SParseTerm>(lexem.data))
            ++term_cnt;
    }

    ok = flexx_includes(out_stream);
    if (!ok) return false;
    
    ok = flexx_structure(out_stream);
    if (!ok) return false;

    out_stream << "const std::array<STerm, " << term_cnt << ">";
    out_stream << " = {" "\n";
    out_stream << "\t{\n";
    for (const auto& lexem : lexem_vec)
    {
        if (!std::holds_alternative<SParseTerm>(lexem.data))
            continue;

        ok = flexx_instance(lexem, out_stream);
        if (!ok) return false;

        out_stream << ",\n";
    }
    out_stream << "\t}\n";
    out_stream << "};" "\n";

    for (const auto& lexem : lexem_vec)
    {
        if (!std::holds_alternative<SParseTerm>(lexem.data))
            continue;

        ok = flexx_function(lexem, out_stream);
        if (!ok) return false;
    }

    return true;
}

template<typename TStream>
bool CFlexx::flexx_includes(TStream& out_stream)
{
    out_stream << "#include <vector>" "\n";
    out_stream << "#include \"STerm.h\"" "\n";

    return true;
}

template<typename TStream>
bool CFlexx::flexx_structure(TStream& out_stream)
{
    out_stream << "struct STerm" "\n";
    out_stream << "{\n";
    out_stream << "    std::string_view name;\n";
    out_stream << "    std::regex expr;\n";
    out_stream << "};\n";

    return true;
}

template<typename TStream>
bool CFlexx::flexx_instance(const SParseLexem& lexem,
                            TStream& out_stream)
{
    if (!std::holds_alternative<SParseTerm>(lexem.data))
        return false;

    SParseTerm term = std::get<SParseTerm>(lexem.data);

    out_stream << "\t";
    out_stream << "{ ";
    out_stream << term.name_view;
    out_stream << ", ";
    out_stream << "std::regex(" << term.expr_view << ") ";
    out_stream << "}";
    
    return true;
}

template<typename TStream>
bool CFlexx::flexx_function(const SParseLexem& lexem,
                            TStream& out_stream)
{
    if (!std::holds_alternative<SParseTerm>(lexem.data))
        return false;

    SParseTerm term = std::get<SParseTerm>(lexem.data);

    out_stream << "bool " << term.name_view << "_match";
    out_stream << "(const std::string_view& YYTEXT)\n";
    out_stream << "{\n";
    out_stream << "    bool result = false;" "\n";
    out_stream << "    if (std::regex_match(YYTEXT, YYREGEX))" "\n";
    out_stream << "        result = true;" "\n";
    out_stream << "    " "\n";
    out_stream << "    return result;" "\n";
    out_stream << "}\n";

    out_stream << "YYTYPE " << term.name_view << "_value";
    out_stream << "(const std::string_view& YYTEXT)\n";
    out_stream << "{\n";
    out_stream << "    YYTYPE YYLVAL;" "\n";
    out_stream <<      term.code_view << "\n";  
    out_stream << "    return YYLVAL;" "\n";
    out_stream << "}\n";
    
    return true;
}

//} //namespace huyacc

#endif //CFLEXX_H
