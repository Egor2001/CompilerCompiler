#ifndef STERM_H
#define STERM_H

#include <cstdint>

#include <string>
#include <variant>

//namespace {

struct SFlexxTerm
{
    SFlexxTerm() = default;

    template<typename TData>
    SFlexxTerm(const TData& data_set):
        data(data_set)
    {}

    std::variant<std::monostate, std::string, int32_t> data;
};

//} //namespace

#endif //STERM_H
