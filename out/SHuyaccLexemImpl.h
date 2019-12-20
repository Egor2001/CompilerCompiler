#ifndef SHUYACCLEXEMIMPL_H
#define SHUYACCLEXEMIMPL_H

#include "SHuyaccLexem.h"

//namespace {

bool SHuyaccLexXor::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_XOR try\n";
    
    SMatchContext new_context =
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };
    
    bool is_matched = left->try_match(term_vec, new_context);
    if (!is_matched) right->try_match(term_vec, new_context);

    if (is_matched)
    {
        std::cout << "LEX_XOR match\n";
        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);
        context.term_idx = new_context.term_idx;

        result = true;
    }

    return result;
}

bool SHuyaccLexCat::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_CAT try\n";
    
    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    if (left ->try_match(term_vec, new_context) &&
        right->try_match(term_vec, new_context))
    {
        std::cout << "LEX_CAT match\n";
        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);
        context.term_idx = new_context.term_idx;

        result = true;
    }

    return result;
}

bool SHuyaccLexRep::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_REP try\n";

    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    for (auto new_term = left->try_match(term_vec, new_context); new_term;
         new_term = left->try_match(term_vec, new_context))
    {
        std::cout << "LEX_REP match\n";
        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);
        context.term_idx = new_context.term_idx;

        new_context.node_vec.clear();

        result = true;
    }

    return result;
}

bool SHuyaccLexOpt::try_match(const std::vector<SLexem>& term_vec,
                              SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_OPT try\n";

    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    if (auto new_term = left->try_match(term_vec, new_context); new_term)
    {
        std::cout << "LEX_OPT match\n";
        auto beg_it = std::make_move_iterator(new_context.node_vec.begin());
        auto end_it = std::make_move_iterator(new_context.node_vec.end());
        context.node_vec.insert(context.node_vec.cend(), beg_it, end_it);
        context.term_idx = new_context.term_idx;

        result = true;
    }

    return result;
}

bool SHuyaccLexTerm::try_match(const std::vector<SLexem>& term_vec,
                               SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_TERM try\n";

    if (term_vec[context.term_idx].type == term_type)
    {
        std::cout << "LEX_TERM match\n";
        auto term = term_vec[context.term_idx].term;
        auto new_node = std::make_shared<SHuyaccNode>(term);
        context.node_vec.push_back(new_node);
        ++context.term_idx;

        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

bool SHuyaccLexRule::try_match(const std::vector<SLexem>& term_vec,
                               SMatchContext& context) const
{
    bool result = false;
    std::cout << "LEX_RULE try\n";

    SMatchContext new_context =  
    {
        .term_idx = context.term_idx,
        .node_vec = {}
    };

    if (GlobalRuleArray[rule_type].expr->try_match(term_vec, new_context))
    {
        std::cout << "LEX_RULE match\n";
        auto new_node = build_huyacc_rule(rule_type, new_context.node_vec);
        context.node_vec.push_back(new_node);
        context.term_idx = new_context.term_idx;

        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

//}

#endif //SHUYACCLEXEMIMPL_H
