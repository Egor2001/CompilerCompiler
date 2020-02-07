#ifndef CENFAUTOMATON_H
#define CENFAUTOMATON_H

#include <cstdlib>
#include <cstring>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <string_view>

//namespace {

template<typename TSymb>
class CENFAutomaton;

bool enfa_read_str(CENFAutomaton<char>& enfa, std::string_view str);

template<typename TSymb>
CENFAutomaton<TSymb> make_str_enfa(const TSymb&);

template<typename TSymb>
CENFAutomaton<TSymb> make_opt_enfa(CENFAutomaton<TSymb>&&);

template<typename TSymb>
CENFAutomaton<TSymb> make_rep_enfa(CENFAutomaton<TSymb>&&);

template<typename TSymb>
CENFAutomaton<TSymb> make_sum_enfa(CENFAutomaton<TSymb>&&, 
                                   CENFAutomaton<TSymb>&&);

template<typename TSymb>
CENFAutomaton<TSymb> make_cat_enfa(CENFAutomaton<TSymb>&&, 
                                   CENFAutomaton<TSymb>&&);

template<typename TSymb>
class CENFAutomaton
{
public:
    struct SNode;
    using TIter = typename std::list<SNode>::iterator; 

    //TODO: oh, fuck... (works only for valid iterators) 
    struct SNodeListItHash
    {
        std::size_t operator () (const TIter& it) const
        {
            return reinterpret_cast<std::size_t>(&(*it));
        }
    };

    struct SNode
    {
        std::unordered_set<TIter, SNodeListItHash> eps_edge_set;
        std::unordered_map<TSymb, TIter> edge_map;
    };

public:
    CENFAutomaton();
    explicit CENFAutomaton(std::list<SNode>&&);

    CENFAutomaton             (const CENFAutomaton&) = delete;
    CENFAutomaton& operator = (const CENFAutomaton&) = delete;

    CENFAutomaton             (CENFAutomaton&&) = default;
    CENFAutomaton& operator = (CENFAutomaton&&) = default;

    ~CENFAutomaton() = default;

    TIter start_it()     { return node_list_.begin(); }
    TIter accepting_it() { return std::prev(node_list_.end()); }

          std::list<SNode>& node_list()       noexcept { return node_list_; }
    const std::list<SNode>& node_list() const noexcept { return node_list_; }

    std::unordered_set<TIter, SNodeListItHash>
    next_enfa_eps(TIter);

    std::unordered_set<TIter, SNodeListItHash>
    next_enfa_set(TIter, const TSymb&);

    std::unordered_map<TIter, std::size_t, SNodeListItHash> 
    node_list_map();

    std::vector<TIter> 
    node_list_vec();

private:
    std::list<SNode> node_list_;
};

template<typename TSymb>
CENFAutomaton<TSymb>::CENFAutomaton():
    node_list_(std::list<SNode>(2u))
{}

template<typename TSymb>
CENFAutomaton<TSymb>::CENFAutomaton(std::list<SNode>&& move_node_list):
    node_list_(std::move(move_node_list))
{}

template<typename TSymb>
std::unordered_set<typename CENFAutomaton<TSymb>::TIter, 
                   typename CENFAutomaton<TSymb>::SNodeListItHash>
CENFAutomaton<TSymb>::next_enfa_eps(CENFAutomaton<TSymb>::TIter it)
{
    std::unordered_set<TIter, SNodeListItHash> result;
    result.insert(it);

    std::stack<TIter> dfs_stack;
    dfs_stack.push(it);

    while (!dfs_stack.empty())
    {
        TIter cur_it = dfs_stack.top();
        dfs_stack.pop();

        for (auto& new_it : cur_it->eps_edge_set)
        {
            if (!result.count(new_it))
            {
                result.insert(new_it);
                dfs_stack.push(new_it);
            }
        }
    }

    return result;
}

template<typename TSymb>
std::unordered_set<typename CENFAutomaton<TSymb>::TIter, 
                   typename CENFAutomaton<TSymb>::SNodeListItHash>
CENFAutomaton<TSymb>::next_enfa_set(CENFAutomaton<TSymb>::TIter it, 
                                    const TSymb& symb)
{
    std::unordered_set<TIter, SNodeListItHash> result;

    auto eps_set = next_enfa_eps(it);
    for (const auto& cur_it : eps_set)
    {
        auto [begin_map_it, end_map_it] = cur_it->edge_map.equal_range(symb);
        for (auto map_it = begin_map_it; map_it != end_map_it; ++map_it)
            result.insert(map_it->second);
    }

    return result;
}

template<typename TSymb>
std::unordered_map<typename CENFAutomaton<TSymb>::TIter, std::size_t, 
                   typename CENFAutomaton<TSymb>::SNodeListItHash> 
CENFAutomaton<TSymb>::node_list_map()
{
    std::unordered_map<TIter, std::size_t, SNodeListItHash> result;

    std::size_t cur_pos = 0u;
    for (auto it = node_list_.begin(); it != node_list_.end(); ++it)
    {
        result.insert(std::make_pair(it, cur_pos));
        ++cur_pos;
    }

    return result;
}

template<typename TSymb>
std::vector<typename CENFAutomaton<TSymb>::TIter> 
CENFAutomaton<TSymb>::node_list_vec()
{
    auto result = std::vector<TIter>(node_list_.size());

    std::size_t cur_pos = 0u;
    for (auto it = node_list_.begin(); it != node_list_.end(); ++it)
    {
        result[cur_pos] = it;
        ++cur_pos;
    }

    return result;
};

template<typename TSymb>
CENFAutomaton<TSymb> make_str_enfa(const TSymb& symb)
{
    auto node_list = std::list<typename CENFAutomaton<TSymb>::SNode>(2u);

    auto start_it = node_list.begin(); 
    auto accepting_it = std::prev(node_list.end());

    start_it->edge_map.insert(std::make_pair(symb, accepting_it));

    return CENFAutomaton<TSymb>(std::move(node_list));
}

template<typename TSymb>
CENFAutomaton<TSymb> make_opt_enfa(CENFAutomaton<TSymb>&& root_enfa)
{
    auto node_list = std::move(root_enfa.node_list());
    node_list.emplace_front();
    node_list.emplace_back();

    auto new_start_it = node_list.begin(); 
    auto old_start_it = std::next(new_start_it);
    auto new_accepting_it = std::prev(node_list.end());
    auto old_accepting_it = std::prev(new_accepting_it);

    new_start_it->eps_edge_set.insert(old_start_it);
    new_start_it->eps_edge_set.insert(new_accepting_it);
    old_accepting_it->eps_edge_set.insert(new_accepting_it);

    return CENFAutomaton<TSymb>(std::move(node_list)); 
}

template<typename TSymb>
CENFAutomaton<TSymb> make_rep_enfa(CENFAutomaton<TSymb>&& root_enfa)
{
    auto node_list = std::move(root_enfa.node_list());
    node_list.emplace_front();
    node_list.emplace_back();

    auto new_start_it = node_list.begin(); 
    auto old_start_it = std::next(new_start_it);
    auto new_accepting_it = std::prev(node_list.end());
    auto old_accepting_it = std::prev(new_accepting_it);

    new_start_it->eps_edge_set.insert(old_start_it);
    new_start_it->eps_edge_set.insert(new_accepting_it);
    old_accepting_it->eps_edge_set.insert(new_accepting_it);
    old_accepting_it->eps_edge_set.insert(old_start_it);

    return CENFAutomaton<TSymb>(std::move(node_list)); 
}

template<typename TSymb>
CENFAutomaton<TSymb> make_sum_enfa(CENFAutomaton<TSymb>&& left_enfa, 
                                   CENFAutomaton<TSymb>&& right_enfa)
{
    auto node_list = std::list<typename CENFAutomaton<TSymb>::SNode>(2u);

    auto left_list = std::move(left_enfa.node_list());
    auto right_list = std::move(right_enfa.node_list());

    auto new_start_it = node_list.begin();
    auto left_start_it = left_list.begin();
    auto right_start_it = right_list.begin();

    auto new_accepting_it = std::prev(node_list.end());
    auto left_accepting_it = std::prev(left_list.end());
    auto right_accepting_it = std::prev(right_list.end());

    auto splice_it = std::next(node_list.begin());
    node_list.splice(splice_it, std::move(left_list));
    node_list.splice(splice_it, std::move(right_list));

    new_start_it->eps_edge_set.insert(left_start_it);
    new_start_it->eps_edge_set.insert(right_start_it);
    left_accepting_it->eps_edge_set.insert(new_accepting_it);
    right_accepting_it->eps_edge_set.insert(new_accepting_it);

    return CENFAutomaton<TSymb>(std::move(node_list)); 
}

template<typename TSymb>
CENFAutomaton<TSymb> make_cat_enfa(CENFAutomaton<TSymb>&& left_enfa, 
                                   CENFAutomaton<TSymb>&& right_enfa)
{
    auto node_list = std::list<typename CENFAutomaton<TSymb>::SNode>();

    auto left_list = std::move(left_enfa.node_list());
    auto right_list = std::move(right_enfa.node_list());

    auto left_accepting_it = std::prev(left_list.end());
    auto right_start_it = right_list.begin();

    auto splice_it = node_list.end();
    node_list.splice(splice_it, std::move(left_list));
    node_list.splice(splice_it, std::move(right_list));

    left_accepting_it->eps_edge_set.insert(right_start_it);

    return CENFAutomaton<TSymb>(std::move(node_list)); 
}

bool enfa_read_str(CENFAutomaton<char>& enfa, std::string_view str)
{
    bool result = true;

    std::unordered_set<CENFAutomaton<char>::TIter, 
                       CENFAutomaton<char>::SNodeListItHash> old_set;
    old_set = enfa.next_enfa_eps(enfa.start_it());

    std::unordered_set<CENFAutomaton<char>::TIter, 
                       CENFAutomaton<char>::SNodeListItHash> new_set;
    for (char symb : str)
    {
        for (const auto& it : old_set)
        {
            auto it_set = enfa.next_enfa_set(it, symb);
            for (const auto& sub_it : it_set)
                new_set.merge(enfa.next_enfa_eps(sub_it));
        }

        old_set = std::move(new_set);
        new_set.clear();
    }

    result = (old_set.count(enfa.accepting_it()));

    return result;
}

//} //namespace 

#endif //CENFAUTOMATON_H
