#ifndef STERM_H
#define STERM_H

#include <initializer_list>
#include <string_view>
#include <regex>

//namespace {

enum ETermType;
enum ERuleType;

struct STerm;
struct SRule;

struct SLexCat;
struct SLexXor;
struct SLexRep;

struct SLexem;

struct STerm
{
    bool match(const std::string_view& text)
    {
        return std::match(text, expr);
    }

    ETermType  type;
    std::regex expr;
};

struct SRule
{
    bool match(const std::string_view& text)
    {
        return std::match(text, expr);
    }

    ERuleType type;
};

struct SLexCat
{
    //because of lightweight copying
    SLexCat(std::initializer_list<std::shared_ptr<SLexem>> init):
        child_vec_(init)
    {}

    bool apply(const std::vector<SToken>& tok_vec)
    {
        bool result = true;
        for (const auto& child : child_vec_)
        {
            result = result && child.apply(tok_vec);
        }
    }

    std::vector<std::shared_ptr<SLexem>> child_vec_;
};

struct SLexXor;
struct SLexRep;

//} //namespace

#endif //STERM_H
