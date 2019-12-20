#ifndef SHUYACCLEXEM_H
#define SHUYACCLEXEM_H

#include <memory>
#include <vector>
#include <variant>
#include <iterator>

#include "SHuyaccNode.h"

//namespace {

struct SMatchContext
{
    std::size_t term_idx;
    std::vector<std::shared_ptr<SHuyaccNode>> node_vec;
};

struct SHuyaccLexem;

struct SHuyaccLexXor
{
    SHuyaccLexXor(const std::shared_ptr<SHuyaccLexem>& left_set,
                  const std::shared_ptr<SHuyaccLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    std::shared_ptr<SHuyaccLexem> left;
    std::shared_ptr<SHuyaccLexem> right;
};

struct SHuyaccLexCat
{
    SHuyaccLexCat(const std::shared_ptr<SHuyaccLexem>& left_set,
                  const std::shared_ptr<SHuyaccLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    std::shared_ptr<SHuyaccLexem> left;
    std::shared_ptr<SHuyaccLexem> right;
};

struct SHuyaccLexRep
{
    explicit SHuyaccLexRep(const std::shared_ptr<SHuyaccLexem>& left_set):
        left(left_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    std::shared_ptr<SHuyaccLexem> left;
};

struct SHuyaccLexOpt
{
    explicit SHuyaccLexOpt(const std::shared_ptr<SHuyaccLexem>& left_set):
        left(left_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    std::shared_ptr<SHuyaccLexem> left;
};

struct SHuyaccLexTerm
{
    explicit SHuyaccLexTerm(ETermType term_type_set):
        term_type(term_type_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    ETermType term_type;
};
/*
struct SHuyaccLexRule
{
    explicit SHuyaccLexRule(ERuleType rule_type_set):
        rule_type(rule_type_set)
    {}

    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const;

    EHuyaccRule rule_type;
};
*/
struct SHuyaccLexem
{
    template<typename TData>
    SHuyaccLexem(const TData& data_set):
        data(data_set)
    {}
     
    bool try_match(const std::vector<SLexem>& term_vec,
                   SMatchContext& context) const
    {
        auto visitor = [this, &term_vec, &context](const auto& data)
        {
            if constexpr (!std::is_same_v<decltype(data), 
                                          const std::monostate&>)
            {
                return data.try_match(term_vec, context);
            }

            return false;
        };

        return std::visit(visitor, data);
    }

    std::variant<std::monostate, SHuyaccLexTerm, /*SHuyaccLexRule,*/
                 SHuyaccLexXor, SHuyaccLexCat,
                 SHuyaccLexRep, SHuyaccLexOpt> data;
};

bool SHuyaccLexXor::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    
    SMatchContext new_context =
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };
    
    if (left ->try_match(term_vec, new_context) || 
        right->try_match(term_vec, new_context))
    {
        context.term_idx = new_context.term_idx;
        context.node_vec = std::move(new_context.node_vec);
    }

    return result;
}

bool SHuyaccLexCat::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    
    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    if (left ->try_match(term_vec, new_context) &&
        right->try_match(term_vec, new_context))
    {
        context.term_idx = new_context.term_idx;
        context.node_vec = std::move(new_context.node_vec);
    }

    return result;
}

bool SHuyaccLexRep::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;

    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    for (auto new_term = left->try_match(term_vec, new_context); new_term;
         new_term = left->try_match(term_vec, new_context))
    {
        context.term_idx = new_context.term_idx;

        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);

        new_context.node_vec.clear();
    }

    return result;
}

bool SHuyaccLexOpt::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;

    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    if (auto new_term = left->try_match(term_vec, new_context); new_term)
    {
        context.term_idx = new_context.term_idx;

        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);
    }

    return result;
}

bool SHuyaccLexTerm::try_match(const std::vector<SLexem>& term_vec,
                               SMatchContext& context) const
{
    bool result = false;

    if (term_vec[context.term_idx].type == term_type)
    {
        auto term = term_vec[context.term_idx].term;
        auto new_node = std::make_shared<SHuyaccNode>(term);
        context.node_vec.push_back(new_node);
    }
    else
    {
        result = false;
    }

    return result;
}

//}

#endif //SHUYACCLEXEM_H
