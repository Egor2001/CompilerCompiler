#ifndef SLEXEM_H
#define SLEXEM_H

#include <variant>
#include <vector>
#include <shared_ptr>

//namespace {

template<typename... TVisitors>
struct SMultiVisitor : TVisitors...
{
    using TVisitors::operator()...;
};

template<typename... TVisitors>
SMultiVisitor(TVisitors... visitors) -> 
    SMultiVisitor<TVisitors...>;

struct SLexem;

struct SLexTerm
{
    explicit SLexTerm(const std::string_view& name_view_set)
        name_view(name_view_set)
    {}

    void build_parse(std::string& out_str) const
    {
        out_str.append("std::make_shared<SLexTerm>(");
        out_str.append(name_view);
        out_str.append(")");
    }

    std::string_view name_view;
};

struct SLexConcat
{
    SLexConcat(const std::shared_ptr<SLexem>& left_set,
               const std::shared_ptr<SLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    void build_parse(std::string& out_str) const
    {
        out_str.append("std::make_shared<SLexConcat>(");

        if (left) left->build_parse(out_str);
        else      out_str.append("nullptr");

        out_str.append(", ");

        if (right) right->build_parse(out_str);
        else       out_str.append("nullptr");

        out_str.append(")");
    }

    std::shared_ptr<SLexem> left; 
    std::shared_ptr<SLexem> right; 
};

struct SLexChoise
{
    SLexChoise(const std::shared_ptr<SLexem>& left_set,
               const std::shared_ptr<SLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    void build_parse(std::string& out_str) const
    {
        out_str.append("std::make_shared<SLexChoise>(");

        if (left) left->build_parse(out_str);
        else      out_str.append("nullptr");

        out_str.append(", ");

        if (right) right->build_parse(out_str);
        else       out_str.append("nullptr");

        out_str.append(")");
    }

    std::shared_ptr<SLexem> left; 
    std::shared_ptr<SLexem> right; 
};

struct SLexRepeat
{
    explicit SLexRepeat(const std::shared_ptr<SLexem>& left_set):
        left(left_set)
    {}

    void build_parse(std::string& out_str) const
    {
        out_str.append("std::make_shared<SLexRepeat>(");

        if (left) left->build_parse(out_str);
        else      out_str.append("nullptr");

        out_str.append(")");
    }

    std::shared_ptr<SLexem> left; 
};

struct SLexOption
{
    explicit SLexOption(const std::shared_ptr<SLexem>& left_set):
        left(left_set)
    {}

    void build_parse(std::string& out_str) const
    {
        out_str.append("std::make_shared<SLexem>(SLexOption(");

        if (left) left->build_parse(out_str);
        else      out_str.append("nullptr");

        out_str.append(")");
    }

    std::shared_ptr<SLexem> left; 
};

struct SLexem
{
    template<typename TData>
    explicit SLexem(const TData& data_set):
       data(data_set) 
    {}

    void build_parse(std::string& out_str) const
    {
        auto visitor = SMultiVisitor 
        {
            [&out_str](const std::monostate&) {},
            [&out_str](const auto& lex) mutable { lex.build_parse(out_str); },
        };

        std::visit(data, visitor);
    }

    std::variant<std::monostate, SLexTerm,
                 SLexConcat, SLexChoise, 
                 SLexRepeat, SLexOption> data;
};

class CRuleParser
{
public:
    CRuleParser() = default;

    std::shared_ptr<SLexem> 
    parse(const std::string_view& rule_view_set);

    std::shared_ptr<SLexem> parse_regex();

    std::shared_ptr<SLexem> parse_choise();
    std::shared_ptr<SLexem> parse_concat();

    std::shared_ptr<SLexem> parse_group();
    std::shared_ptr<SLexem> parse_token();

    bool skip(const std::string_view& skip_view);

private:
    std::string_view rule_view_;
};

std::shared_ptr<SLexem> 
CRuleParser::parse(const std::string_view& rule_view_set)
{
    rule_view_ = rule_view_set; 

    return parse_regex();
}

std::shared_ptr<SLexem> CRuleParser::parse_regex()
{
    return parse_choise();
}

std::shared_ptr<SLexem> CRuleParser::parse_choise()
{
    std::shared_ptr<SLexem> result;

    auto left = parse_concat();
    auto right = std::shared_ptr<SLexem>();
    while (skip("|"))
    {
        right = parse_concat();
        if (!right) return result;

        left = std::make_shared<SLexem>(SChoise(left, right));
    }

    result = left;

    return result;
}

std::shared_ptr<SLexem> CRuleParser::parse_concat()
{
    std::shared_ptr<SLexem> result;

    auto left = parse_group();
    auto right = std::shared_ptr<SLexem>();
    while (true)
    {
        right = parse_group();
        if (!right) return result;

        left = std::make_shared<SLexem>(SChoise(left, right));
    }

    result = left;

    return result;
}

std::shared_ptr<SLexem> CRuleParser::parse_group()
{
    std::shared_ptr<SLexem> result;
    std::shared_ptr<SLexem> left;

    if (skip("{"))
    {
        left = parse_regex();
        if (!skip("}")) return result;

        result = std::make_shared<SLexem>(SLexRepeat(left));
    }
    else if (skip("["))
    {
        left = parse_regex();
        if (!skip("]")) return result;

        result = std::make_shared<SLexem>(SLexOption(left));
    }
    else if (skip("("))
    {
        left = parse_regex();
        if (!skip(")")) return result;

        result = left;
    }
    else
    {
        left = parse_token();
        result = left;
    }

    return result;
}

std::shared_ptr<SLexem> CRuleParser::parse_group()
{
    std::shared_ptr<SLexem> result;
    std::shared_ptr<SLexem> left;

    if (skip("\""))
    {
        left = parse_regex();
        if (!skip("\"")) return result;

        result = std::make_shared<SLexem>(SLexRepeat(left));
    }
    else if (skip("<"))
    {
        left = parse_regex();
        if (!skip(">")) return result;

        result = std::make_shared<SLexem>(SLexOption(left));
    }

    return result;
}

bool CRuleParser::skip(const std::string_view& skip_view)
{
    bool result = false;
    if (rule_view_.compare(0u, skip_view.size(), skip_view) == 0)
    {
        rule_view_.remove_prefix(skip_view.size());

        std::size_t skip_len = rule_view_.first_not_of(" \t\r\n");
        rule_view_.remove_prefix(std::min(skip_len, rule_view_.size()));

        result = true;
    }

    return result;
}

//} //namespace

#endif //SLEXEM_H
