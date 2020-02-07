#ifndef CDFAUTOMATON_H
#define CDFAUTOMATON_H

#include <cstdint>

#include <unordered_map>
#include <vector>

//namespace {

template<typename TSymb>
class CDFAutomaton
{
public:
    explicit CDFAutomaton(std::size_t node_cnt_set);
    CDFAutomaton(std::size_t node_cnt_set, 
                 const std::vector<bool>& accepting_mask_set);
    CDFAutomaton(std::size_t node_cnt_set, 
                 std::vector<bool>&& accepting_mask_set);

    CDFAutomaton             (const CDFAutomaton&) = default;
    CDFAutomaton& operator = (const CDFAutomaton&) = default;

    CDFAutomaton             (CDFAutomaton&&) = default;
    CDFAutomaton& operator = (CDFAutomaton&&) = default;

    ~CDFAutomaton() = default;

    [[nodiscard]]
    std::size_t node_cnt() const noexcept { return node_cnt_; }

    [[nodiscard]]
    std::size_t next(std::size_t node, const TSymb& symb) const;

    CDFAutomaton& 
    set_edge_vec(const TSymb& symb, 
                 const std::vector<std::size_t>& edge_vec_set);
    CDFAutomaton& 
    set_edge_vec(const TSymb& symb, 
                 std::vector<std::size_t>&& edge_vec_set);

    CDFAutomaton& 
    set_accepting_mask(const std::vector<bool>& accepting_mask_set); 
    CDFAutomaton& 
    set_accepting_mask(std::vector<bool>&& accepting_mask_set); 

private:
    std::size_t node_cnt_;
    std::unordered_map<TSymb, std::vector<uint64_t>> edge_vec_map_;
    std::vector<bool> accepting_mask_;
};

template<typename TSymb>
CDFAutomaton<TSymb>::
CDFAutomaton(std::size_t node_cnt_set):
    node_cnt_(node_cnt_set),
    edge_vec_map_(),
    accepting_mask_(node_cnt_)
{}

template<typename TSymb>
CDFAutomaton<TSymb>::
CDFAutomaton(std::size_t node_cnt_set,
             const std::vector<bool>& accepting_mask_set):
    node_cnt_(node_cnt_set),
    edge_vec_map_(),
    accepting_mask_(accepting_mask_set)
{}

template<typename TSymb>
CDFAutomaton<TSymb>::
CDFAutomaton(std::size_t node_cnt_set,
             std::vector<bool>&& accepting_mask_set):
    node_cnt_(node_cnt_set),
    edge_vec_map_(),
    accepting_mask_(std::move(accepting_mask_set))
{}

template<typename TSymb>
std::size_t 
CDFAutomaton<TSymb>::
next(std::size_t node, const TSymb& symb) const
{
    std::size_t result = static_cast<std::size_t>(-1);

    auto it = edge_vec_map_.find(symb);
    if (it != edge_vec_map_.end() && node < node_cnt_)
        result = it->second[node];

    return result;
}

template<typename TSymb>
CDFAutomaton<TSymb>&
CDFAutomaton<TSymb>::
set_edge_vec(const TSymb& symb, 
             const std::vector<std::size_t>& edge_vec_set)
{
    edge_vec_map_[symb] = edge_vec_set;
    return (*this);
}

template<typename TSymb>
CDFAutomaton<TSymb>&
CDFAutomaton<TSymb>::
set_edge_vec(const TSymb& symb, 
             std::vector<std::size_t>&& edge_vec_set)
{
    edge_vec_map_[symb] = std::move(edge_vec_set);
    return (*this);
}

template<typename TSymb>
CDFAutomaton<TSymb>&
CDFAutomaton<TSymb>::
set_accepting_mask(const std::vector<bool>& accepting_mask_set)
{
    accepting_mask_ = accepting_mask_set;
    return (*this);
}

template<typename TSymb>
CDFAutomaton<TSymb>&
CDFAutomaton<TSymb>::
set_accepting_mask(std::vector<bool>&& accepting_mask_set)
{
    accepting_mask_ = std::move(accepting_mask_set);
    return (*this);
}

//} //namespace

#endif //CDFAUTOMATON_H
