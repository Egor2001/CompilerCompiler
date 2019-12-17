#ifndef SBUILDER_H
#define SBUILDER_H

#include <variant>
#include <vector>

#include "SLexem.h"

//namespace {

class CBuilder
{
public:
    CBuilder() = default;

    std::shared_ptr<SLexem> 
    parse(const std::string_view& rule_view_set);

private:
    std::shared_ptr<SLexem> parse_regex();

    std::shared_ptr<SLexem> parse_xor();
    std::shared_ptr<SLexem> parse_cat();

    std::shared_ptr<SLexem> parse_and();
    std::shared_ptr<SLexem> parse_tok();

    std::string_view parse_name();

    bool skip(const std::string_view& skip_view);

private:
    std::string_view rule_view_;
};

std::shared_ptr<SLexem> 
CBuilder::parse(const std::string_view& rule_view_set)
{
    rule_view_ = rule_view_set; 

    return parse_regex();
}

std::shared_ptr<SLexem> CBuilder::parse_regex()
{
    return parse_xor();
}

std::shared_ptr<SLexem> CBuilder::parse_xor()
{
    std::shared_ptr<SLexem> result;

    auto left = parse_cat();
    auto right = std::shared_ptr<SLexem>();
    while (skip("|"))
    {
        right = parse_cat();
        if (!right) return result;

        left = std::make_shared<SLexem>(SLexXor(left, right));
    }

    result = left;

    return result;
}

std::shared_ptr<SLexem> CBuilder::parse_cat()
{
    std::shared_ptr<SLexem> result;

    auto left = parse_and();
    auto right = std::shared_ptr<SLexem>();

    while (true)
    {
        right = parse_and();
        if (!right) break;

        left = std::make_shared<SLexem>(SLexCat(left, right));
    }

    result = left;

    return result;
}

std::shared_ptr<SLexem> CBuilder::parse_and()
{
    std::shared_ptr<SLexem> result;
    std::shared_ptr<SLexem> left;

    if (skip("{"))
    {
        left = parse_regex();
        if (!skip("}")) return result;

        result = std::make_shared<SLexem>(SLexRep(left));
    }
    else if (skip("["))
    {
        left = parse_regex();
        if (!skip("]")) return result;

        result = std::make_shared<SLexem>(SLexOpt(left));
    }
    else if (skip("("))
    {
        left = parse_regex();
        if (!skip(")")) return result;

        result = left;
    }
    else
    {
        left = parse_tok();
        result = left;
    }

    return result;
}

std::shared_ptr<SLexem> CBuilder::parse_tok()
{
    std::shared_ptr<SLexem> result;
    std::string_view name_view;

    if (skip("<"))
    {
        name_view = parse_name();
        if (!skip(">")) return result;

        result = std::make_shared<SLexem>(SLexTerm(name_view));
    }
    else
        return nullptr;

    return result;
}

std::string_view CBuilder::parse_name()
{
    std::string_view result;
    std::size_t name_len = rule_view_.find_first_of(">");

    if (name_len >= rule_view_.size())
        return result;

    result = rule_view_.substr(0u, name_len);
    rule_view_.remove_prefix(name_len);

    return result;
}

bool CBuilder::skip(const std::string_view& skip_view)
{
    bool result = false;
    if (rule_view_.compare(0u, skip_view.size(), skip_view) == 0)
    {
        rule_view_.remove_prefix(skip_view.size());

        std::size_t skip_len = rule_view_.find_first_not_of(" \t\r\n");
        rule_view_.remove_prefix(std::min(skip_len, rule_view_.size()));

        result = true;
    }

    return result;
}

//} //namespace

#endif //SBUILDER_H
