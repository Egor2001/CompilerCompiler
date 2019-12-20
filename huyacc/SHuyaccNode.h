#ifndef SHUYACCNODE_H
#define SHUYACCNODE_H

#include <string>
#include <memory>
#include <variant>

//namespace {

struct SHuyaccNode
{
    SHuyaccNode() = default;

    template<typename TData>
    SHuyaccNode(const TData& data_set,
                std::shared_ptr<SHuyaccNode> left_set = nullptr,
                std::shared_ptr<SHuyaccNode> right_set = nullptr):
        data(data_set),
        left(left_set),
        right(right_set)
    {}

    std::variant<std::monostate, std::string, SFlexxTerm> data;

    std::shared_ptr<SHuyaccNode> left;
    std::shared_ptr<SHuyaccNode> right;
};

//}

#endif //SHUYACCNODE_H
