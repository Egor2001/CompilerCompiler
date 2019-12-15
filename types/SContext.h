#ifndef SCONTEXT_H
#define SCONTEXT_H

//namespace {

struct STerm
{
    std::string name; 
    std::string expr;
};

struct SSkip
{
    std::string name;
};

struct SContext
{
    void build_source_term_enum (std::string& out_str) const;
    void build_source_skip_array(std::string& out_str) const;

    std::unordered_map<STerm, std::size_t> term_map;
    std::unordered_map<SSkip, std::size_t> skip_map;
};

void SContext::build_source_term_enum(std::string& out_str) const
{
    auto term_vec = std::vector<STerm>(term_map.size());

    for (const auto& [term, idx] : term_map) 
        term_vec[idx] = term;

    out_str.append("const std::array<STerm, > GlobalTermArray");
    //must iterate from head to tail
    for (const auto& term : term_vec)
    {
        term.buil
    }
}

//} //namespace

#endif //SCONTEXT_H
