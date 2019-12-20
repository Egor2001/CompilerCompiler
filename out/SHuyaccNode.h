#ifndef SHUYACCNODE_H
#define SHUYACCNODE_H

#include <string>
#include <memory>
#include <variant>

//namespace {

struct SHuyaccNode
{
    SHuyaccNode() = default;

    SHuyaccNode(const SFlexxTerm& term_set,
                std::shared_ptr<SHuyaccNode> left_set = nullptr,
                std::shared_ptr<SHuyaccNode> right_set = nullptr):
        term(term_set),
        left(left_set),
        right(right_set)
    {}

    SFlexxTerm term;
    std::shared_ptr<SHuyaccNode> left;
    std::shared_ptr<SHuyaccNode> right;
};

//}

#endif //SHUYACCNODE_H
