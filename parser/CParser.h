#ifndef CPARSER_H
#define CPARSER_H

#include <vector>
#include <optional>
#include <string_view>

#include "SParseTerm.h"
#include "SParseRule.h"

//namespace huyacc {

class CParser
{
public:
    CParser() = default;

    bool parse(const std::string_view& text_view);

    const std::vector<SParseTerm>& term_vec() const noexcept;
    const std::vector<SParseRule>& rule_vec() const noexcept;

    template<typename TStream>
    void print(TStream& out_stream) const;

private:
    bool parse_init();
    bool parse_body();

    bool parse_term();
    bool parse_rule();

    std::optional<std::string_view> parse_name();
    std::optional<std::string_view> parse_expr();
    std::optional<std::string_view> parse_code();

    bool parse_skip(const std::string_view& skip_view);
    void parse_void();

private:
    std::vector<SParseTerm> term_vec_;
    std::vector<SParseRule> rule_vec_;

    std::string_view text_view_;
};

bool CParser::parse(const std::string_view& text_view_set)
{
    text_view_ = text_view_set;

    bool is_init = parse_init();
    if (!is_init)
    {
        term_vec_.clear();
        return false;
    }

    bool is_body = parse_body();
    if (!is_body)
    {
        rule_vec_.clear();
        return false;
    }

    return true;
}

const std::vector<SParseTerm>& 
CParser::term_vec() const noexcept
{
    return term_vec_;
}

const std::vector<SParseRule>& 
CParser::rule_vec() const noexcept
{
    return rule_vec_;
}

template<typename TStream>
void CParser::print(TStream& out_stream) const
{
    for (const auto& term : term_vec_)
    {
        out_stream << "(TERM <" << term.name_view << "> " <<
                   term.expr_view << "\n" 
                   "{" << 
                   term.code_view << 
                   "})\n";
    }

    for (const auto& rule : rule_vec_)
    {
        out_stream << "(RULE <" << rule.name_view << "> ::= " <<
                   rule.expr_view << "\n" 
                   "{" << 
                   rule.code_view << 
                   "})\n";
    }
}

bool CParser::parse_init()
{
    bool is_parsed = true;

    parse_void();
    if (!parse_skip("%{")) return false;
    while (is_parsed) 
    {
        is_parsed = parse_term();
    }
    if (!parse_skip("%}")) return false;

    return true;
}

bool CParser::parse_body()
{
    bool is_parsed = true;

    parse_void();
    if (!parse_skip("%{")) return false;
    while (is_parsed) 
    {
        is_parsed = parse_rule();
    }
    if (!parse_skip("%}")) return false;

    return true;
}

bool CParser::parse_term()
{
    SParseTerm term = {};
    //std::cout << "term" "\n";

    term.name_view = parse_name().value_or("");
    if (term.name_view.empty()) return false;
    //std::cout << "term.name" "\n";

    term.expr_view = parse_expr().value_or("");
    if (term.expr_view.empty()) return false;
    //std::cout << "term.expr" "\n";

    term.code_view = parse_code().value_or("");
    //std::cout << "term.code" "\n";

    term_vec_.push_back(term);

    return true;
}

bool CParser::parse_rule()
{
    SParseRule rule = {};
    //std::cout << "rule" "\n";

    rule.name_view = parse_name().value_or("");
    if (rule.name_view.empty()) return false;
    //std::cout << "rule.name" "\n";

    if (!parse_skip("::=")) return false;
    //std::cout << "rule ::=" "\n";

    rule.expr_view = parse_expr().value_or("");
    if (rule.expr_view.empty()) return false;
    //std::cout << "rule.expr" "\n";

    rule.code_view = parse_code().value_or("");
    //std::cout << "rule.code" "\n";

    rule_vec_.push_back(rule);

    return true;
}

std::optional<std::string_view> 
CParser::parse_name()
{
    std::string_view result;
    //std::cout << "name" "\n";
    
    std::size_t name_len = std::min(text_view_.find_first_of(" \t\n\r"),
                                    text_view_.size());

    result = text_view_.substr(0u, name_len);
    if (result.size() <= 2u ||
        result.front() != '<' || result.back()  != '>')
        return std::nullopt;
    //std::cout << "name.view" "\n";

    result.remove_prefix(1u); //<
    result.remove_suffix(1u); //>

    text_view_.remove_prefix(name_len);
    parse_void();

    return result;
}

std::optional<std::string_view> 
CParser::parse_expr()
{
    std::string_view result;
    
    std::size_t expr_len = std::min(text_view_.find_first_of("\n\r"),
                                    text_view_.size());

    result = text_view_.substr(0u, expr_len);
    if (result.empty())
        return std::nullopt;

    text_view_.remove_prefix(expr_len);
    parse_void();

    return result;
}

std::optional<std::string_view> 
CParser::parse_code()
{
    std::string_view result;

    if (text_view_.find("${", 0u) != 0u)
        return std::nullopt;
    
    std::size_t expr_len = text_view_.find("$}", 0u); //$}
    if (expr_len >= text_view_.size())
        return std::nullopt;

    result = text_view_.substr(0u, expr_len);
    result.remove_prefix(2u); //${

    text_view_.remove_prefix(expr_len + 2u);
    parse_void();

    return result;
}

bool CParser::parse_skip(const std::string_view& skip_view)
{
    bool result = (text_view_.compare(0u, skip_view.size(), skip_view) == 0);

    if (result)
        text_view_.remove_prefix(skip_view.size());
    
    parse_void();

    return result;
}

void CParser::parse_void()
{
    size_t void_len = std::min(text_view_.find_first_not_of(" \t\n\r"),
                               text_view_.size());

    text_view_.remove_prefix(void_len);
}

//} //namespace huyacc

#endif //CPARSER_H
