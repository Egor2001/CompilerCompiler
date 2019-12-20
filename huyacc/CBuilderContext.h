#ifndef CBUILDCONTEXT_H
#define CBUILDCONTEXT_H

#include <string_view>
#include <unordered_set>

//namespace {

class CBuilderContext
{
public:
    CBuilderContext() = default;

    CBuilderContext             (const CBuilderContext&) = delete;
    CBuilderContext& operator = (const CBuilderContext&) = delete;

    CBuilderContext             (CBuilderContext&&);
    CBuilderContext& operator = (CBuilderContext&&);

    bool add_term(const std::string_view& term_name);
    bool add_rule(const std::string_view& rule_name);

    bool is_term(const std::string_view& term_name) const;
    bool is_rule(const std::string_view& rule_name) const;

private:
    std::unordered_set<std::string_view> term_name_set_;
    std::unordered_set<std::string_view> rule_name_set_;
};

CBuilderContext::CBuilderContext(CBuilderContext&& move_context):
    term_name_set_(std::move(move_context.term_name_set_)),
    rule_name_set_(std::move(move_context.rule_name_set_))
{}

CBuilderContext& CBuilderContext::operator = (CBuilderContext&& move_context)
{
    std::swap(term_name_set_, move_context.term_name_set_);
    std::swap(rule_name_set_, move_context.rule_name_set_);

    return *this;
}

bool CBuilderContext::add_term(const std::string_view& term_name)
{
    bool result = false;
    if (!term_name_set_.count(term_name))
    {
        auto [it, is_insert] = term_name_set_.insert(term_name);
        result = is_insert;
    }

    return result;
}

bool CBuilderContext::add_rule(const std::string_view& rule_name)
{
    bool result = false;
    if (!rule_name_set_.count(rule_name))
    {
        auto [it, is_insert] = rule_name_set_.insert(rule_name);
        result = is_insert;
    }

    return result;
}

bool CBuilderContext::is_term(const std::string_view& term_name) const
{
    return term_name_set_.count(term_name);
}

bool CBuilderContext::is_rule(const std::string_view& rule_name) const
{
    return rule_name_set_.count(rule_name);
}

//}

#endif //CBUILDCONTEXT_H
