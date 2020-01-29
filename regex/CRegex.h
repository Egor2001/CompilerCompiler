#ifndef CREGEX_H
#define CREGEX_H

#include <cstdlib>
#include <string_view>
#include <memory>

//namespace {

class CRegex
{
public:
    enum ERegexType
    {
        REGEX_DEL = 0,
        REGEX_STR,
        REGEX_REP,
        REGEX_SUM,
        REGEX_CAT
    };

    struct SRegexNode
    {
        ERegexType type;

        //TODO: to use memory in optimal way (union + struct)
        char data;
        SRegexNode* left;
        SRegexNode* right;
    };

    static SRegexNode* new_regex_str(char data);
    static SRegexNode* new_regex_rep(SRegexNode* node);
    static SRegexNode* new_regex_sum(SRegexNode* left, SRegexNode* right);
    static SRegexNode* new_regex_cat(SRegexNode* left, SRegexNode* right);

    static SRegexNode* new_copy_regex(SRegexNode* root);

    static void delete_regex(SRegexNode* root);

    static CRegex parse_sum(std::string_view );
    static CRegex parse_cat(std::string_view );

public:
    CRegex();
    explicit CRegex(std::string_view regex_str);

    CRegex             (const CRegex&);
    CRegex& operator = (const CRegex&);
    CRegex             (CRegex&&);
    CRegex& operator = (CRegex&&);

    ~CRegex();

    CRegex& add(const CRegex& regex_add);
    CRegex& cat(const CRegex& regex_cat);
    CRegex& rep();

private:
    SRegexNode* root_;
};

CRegex sum(const CRegex& regex_lhs, const CRegex& regex_rhs);
CRegex cat(const CRegex& regex_lhs, const CRegex& regex_rhs);
CRegex rep(const CRegex& regex);

CRegex sum(const CRegex& regex_lhs, const CRegex& regex_rhs)
{
    CRegex result = regex_lhs;
    result.add(regex_rhs);

    return result;
}

CRegex cat(const CRegex& regex_lhs, const CRegex& regex_rhs)
{
    CRegex result = regex_lhs;
    result.cat(regex_rhs);

    return result;
}

CRegex rep(const CRegex& regex)
{
    CRegex result = regex;
    result.rep();

    return result;
}

CRegex::CRegex():
    root_(nullptr)
{}

CRegex::CRegex(std::string_view regex_str):
    root_(nullptr)
{
    auto it = regex_str.begin();
    auto end = regex_str.end();

    if (it != end)
    {
        root_ = new_regex_str(*it);
        ++it;

        while (it != end)
        {
            root_ = new_regex_cat(root_, new_regex_str(*it));
            ++it;
        }
    }
}

CRegex::CRegex(const CRegex& copy_regex):
    root_(new_copy_regex(copy_regex.root_))
{}

CRegex& CRegex::operator = (const CRegex& copy_regex)
{
    if (this == &copy_regex)
        return *this;

    delete_regex(root_);
    root_ = new_copy_regex(copy_regex.root_);

    return *this;
}

CRegex::CRegex(CRegex&& move_regex):
    root_(move_regex.root_)
{
    move_regex.root_ = nullptr;
}

CRegex& CRegex::operator = (CRegex&& move_regex)
{
    if (this == &move_regex)
        return *this;

    std::swap(root_, move_regex.root_);

    return *this;
}

CRegex::~CRegex()
{
    delete_regex(root_);
    root_ = nullptr;
}

CRegex& CRegex::add(const CRegex& regex_add)
{
    root_ = new_regex_sum(root_, new_copy_regex(regex_add.root_));

    return *this;
}

CRegex& CRegex::cat(const CRegex& regex_cat)
{
    root_ = new_regex_cat(root_, new_copy_regex(regex_cat.root_));

    return *this;
}

CRegex& CRegex::rep()
{
    root_ = new_regex_rep(root_);

    return *this;
}

CRegex::SRegexNode* CRegex::new_regex_str(char data)
{
    return new SRegexNode
    { 
        .type = REGEX_STR,
        .data = data, 
        .left = nullptr, 
        .right = nullptr 
    };
}

CRegex::SRegexNode* CRegex::new_regex_rep(SRegexNode* node)
{
    return new SRegexNode
    { 
        .type = REGEX_REP,
        .data = 0, 
        .left = node, 
        .right = nullptr 
    };
}

CRegex::SRegexNode* CRegex::new_regex_sum(SRegexNode* left, SRegexNode* right)
{
    return new SRegexNode
    { 
        .type = REGEX_SUM,
        .data = 0, 
        .left = left, 
        .right = right 
    };
}

CRegex::SRegexNode* CRegex::new_regex_cat(SRegexNode* left, SRegexNode* right)
{
    return new SRegexNode
    { 
        .type = REGEX_CAT,
        .data = 0, 
        .left = left, 
        .right = right 
    };
}

CRegex::SRegexNode* CRegex::new_copy_regex(SRegexNode* root)
{
    SRegexNode* result = nullptr;

    if (root)
    {
        result = new SRegexNode
        {
            .type = root->type,
            .data = root->data,
            .left = new_copy_regex(root->left),
            .right = new_copy_regex(root->right)
        };
    }

    return result;
}

void CRegex::delete_regex(SRegexNode* root)
{
    if (root)
    {
        delete_regex(root->right); 
        delete_regex(root->left); 

        root->right = nullptr;
        root->left = nullptr;

        root->data = 0;
        root->type = REGEX_DEL;
    }

    delete root;
}

//} //namespace

#endif //CREGEX_H
