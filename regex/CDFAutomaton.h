#ifndef CDFAUTOMATON_H
#define CDFAUTOMATON_H

#include <cstdint>
#include <vector>

//namespace {

class CDFAutomaton
{
public:
    static const NULL_EDGE = static_cast<std::size_t>(-1);

    CDFAutomaton(std::size_t alph_size_set, 
                 std::size_t node_cnt_set);

    CDFAutomaton             (const CDFAutomaton&) = default;
    CDFAutomaton& operator = (const CDFAutomaton&) = default;

    CDFAutomaton             (CDFAutomaton&&) = default;
    CDFAutomaton& operator = (CDFAutomaton&&) = default;

    ~CDFAutomaton() = default;

    [[nodiscard]]
    std::size_t size() const noexcept { return node_cnt_; }

    [[nodiscard]]
    std::size_t next(std::size_t node, std::size_t symb) const;

    std::size_t add_node(bool accepting);
    std::size_t add_edge(std::size_t node, 
                         std::size_t dest, uint64_t symb);

    bool set_accepting(std::size_t node, bool value = true);

private:
    std::size_t alph_size_;
    std::size_t node_cnt_;
    std::vector<std::vector<uint64_t>> edge_map_;
    std::vector<bool> accepting_vec_;
};

CDFAutomaton::CDFAutomaton(std::size_t alph_size_set, 
                           std::size_t node_cnt_set):
    alph_size_(alph_size_set),
    node_cnt_(node_cnt_set),
    edge_map_(alph_size_),
    accepting_vec_(alph_size_)
{
    for (auto& symb_map : edge_map_)
    {
        symb_map.resize(node_cnt_);
        for (auto& edge : symb_map)
            edge = NULL_EDGE;
    }
}

std::size_t CDFAutomaton::next(std::size_t node, std::size_t symb) const
{
    return edge_map_[symb][node];
}

std::size_t CDFAutomaton::add_node(bool accepting)
{
    ++node_cnt_;

    accepting_vec_.push_back(accepting);
    for (auto& symb_map : edge_map_)
        symb_map.push_back(NULL_EDGE);

    return node_cnt_;
}

std::size_t CDFAutomaton::add_edge(std::size_t node, 
                                   std::size_t dest, std::size_t symb)
{
    std::size_t old_symb = edge_map_[symb][node];
    edge_map_[symb][node] = dest;

    return old_symb;
}

bool CDFAutomaton::set_accepting(std::size_t node, bool value)
{
    bool old_value = accepting_vec_[node];
    accepting_vec_[node] = value;

    return old_value;
}

//} //namespace

#endif //CDFAUTOMATON_H
