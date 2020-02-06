#ifndef CREGEXPARSER_H
#define CREGEXPARSER_H

#include <cstdlib>

#include "CENFAutomaton.h"

//namespace {

//TODO: to make template alphabet & symbol parser
class CRegexParser
{
public:
    CRegexParser() = default; 

    CRegexParser             (const CRegexParser&) = delete; 
    CRegexParser& operator = (const CRegexParser&) = delete; 

    CRegexParser             (CRegexParser&&) = delete; 
    CRegexParser& operator = (CRegexParser&&) = delete; 

    ~CRegexParser() = default; 

public:
    CENFAutomaton parse(std::string_view parse_str);

private:
    CENFAutomaton parse_sum();
    CENFAutomaton parse_cat();
    CENFAutomaton parse_opt();
    CENFAutomaton parse_rep();
    CENFAutomaton parse_any();

    bool skip_str(std::string_view str);

private:
    std::string_view parse_str_; 
};

CENFAutomaton CRegexParser::parse(std::string_view regex_str)
{
    parse_str_ = regex_str;
    CENFAutomaton result = parse_sum();  

    return result;
}

CENFAutomaton CRegexParser::parse_sum()
{
    CENFAutomaton result = parse_cat();

    CENFAutomaton cur = parse_cat();
    while (cur.root() && skip_str("|"))
    {
        result.make_sum(std::move(cur));
        cur = parse_cat();
    }

    return result;
}

CENFAutomaton CRegexParser::parse_cat()
{
    CENFAutomaton result = parse_any();

    CENFAutomaton cur = parse_any();
    while (cur.root() && !parse_str_.empty())
    {
        result.make_cat(std::move(cur));
        cur = parse_any();
    }

    return result;
}

CENFAutomaton CRegexParser::parse_any()
{
    CENFAutomaton result;

    if (skip_str("("))
    {
        result = parse_sum(); 

        if (!skip_str(")"))
            result = CRegex();
    }
    else if (skip_str("["))
    {
        result = opt(parse_sum()); 

        if (!skip_str("]"))
            result = CRegex();
    }
    else if (skip_str("{"))
    {
        result = rep(parse_sum()); 

        if (!skip_str("}"))
            result = CRegex();
    }
    else 
    {
        std::size_t len = std::min(parse_str_.size(), 
                                   parse_str_.find_first_of("|()[]{}"));

        result = CRegex(parse_str_.substr(0u, len));
        parse_str_.remove_prefix(len);
    }

    return result;
}

bool CRegexParser::skip_str(std::string_view str)
{
    bool result = false;

    if (parse_str_.compare(0u, str.size(), str) == 0)
    {
        result = true;
        parse_str_.remove_prefix(str.size());
    }

    return result;
}

//} //namespace

#endif //CREGEXPARSER_H
