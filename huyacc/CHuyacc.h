#ifndef HCHUYACC_H
#define HCHUYACC_H

#include <vector>

#include "../parser/SParseLexem.h"
#include "CBuilder.h"

//namespace {

class CHuyacc
{
public:
    CHuyacc() = default;

    template<typename TStream>
    bool huyacc(const std::vector<SParseLexem>& lex_vec, 
                TStream& out_stream);

private:
    template<typename TStream>
    bool huyacc_includes(TStream& out_stream);
    template<typename TStream>
    bool huyacc_structure(TStream& out_stream);

    template<typename TStream>
    bool huyacc_enum(const std::vector<SParseLexem>& lex_vec, 
                     TStream& out_stream);
    template<typename TStream>
    bool huyacc_array(const std::vector<SParseLexem>& lex_vec, 
                      TStream& out_stream);

    template<typename TStream>
    bool huyacc_enumitem(const SParseRule& rule, 
                         TStream& out_stream);
    template<typename TStream>
    bool huyacc_instance(const SParseRule& rule, 
                         TStream& out_stream);
    template<typename TStream>
    bool huyacc_function(const SParseRule& rule, 
                         TStream& out_stream);

private:
    CBuilder builder;
};

template<typename TStream>
bool CHuyacc::huyacc(const std::vector<SParseLexem>& lex_vec, 
                     TStream& out_stream)
{
    if (!huyacc_includes(out_stream)) return false;
    if (!huyacc_structure(out_stream)) return false;

    if (!huyacc_enum(lex_vec, out_stream)) return false;
    if (!huyacc_array(lex_vec, out_stream)) return false;

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_includes(TStream& out_stream)
{
    out_stream << "#include <vector>" "\n";
    out_stream << "#include <memory>" "\n";
    out_stream << "#include \"STerm.h\"" "\n";
    out_stream << "#include \"SLexem.h\"" "\n";

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_structure(TStream& out_stream)
{
    out_stream << "struct SRule" "\n"
                  "{\n"
                  "    std::string_view name_view;" "\n"
                  "    std::shared_ptr<SLexem> expr;" "\n"
                  "};\n";

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_enum(const std::vector<SParseLexem>& lex_vec, 
                          TStream& out_stream)
{
    out_stream << "enum ERuleType" "\n";
    out_stream << "{" "\n";

    for (const auto& lexem : lex_vec)
    {
        if (!std::holds_alternative<SParseRule>(lexem.data))
            continue;

        huyacc_enumitem(std::get<SParseRule>(lexem.data), out_stream);
    }

    out_stream << "\t" "RULES_CNT" "\n";
    out_stream << "};" "\n";

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_array(const std::vector<SParseLexem>& lex_vec, 
                           TStream& out_stream)
{
    out_stream << "const std::array<SRule, RULES_CNT> = ";
    out_stream << "{" "\n";
    out_stream << "    {" "\n";

    for (const auto& lexem : lex_vec)
    {
        if (!std::holds_alternative<SParseRule>(lexem.data))
            continue;

        huyacc_instance(std::get<SParseRule>(lexem.data), out_stream);
        out_stream << ",\n";
    }

    out_stream << "    }" "\n";
    out_stream << "};" "\n";

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_enumitem(const SParseRule& rule, 
                              TStream& out_stream)
{
    out_stream << "\t" "RULE_" << rule.name_view << ",\n"; 

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_instance(const SParseRule& rule, 
                              TStream& out_stream)
{
    auto node = builder.parse(rule.expr_view);

    out_stream << "\t{" "\n";
    out_stream << "\t    " << rule.name_view << ",\n"; 
    out_stream << "\t    "; 

    if (node) node->build_parse(out_stream);
    else      out_stream << "nullptr";

    out_stream << "\n";
    out_stream << "\t}";

    return true;
}

template<typename TStream>
bool CHuyacc::huyacc_function(const SParseRule& rule, 
                              TStream& out_stream)
{
    return true;
}

//} //namespace

#endif //HCHUYACC_H
