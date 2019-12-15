#ifndef CPARSELEXEM_H
#define CPARSELEXEM_H

#include <variant>
#include <string_view>

//namespace huyacc {

struct SParseTerm
{
    std::string_view name_view;
    std::string_view expr_view;
    std::string_view code_view;
};

struct SParseRule
{
    std::string_view name_view;
    std::string_view expr_view;
    std::string_view code_view;
};

struct SParseLexem
{
public:
    template<typename TData>
    SParseLexem(const TData& data_set):
        data(data_set)
    {}

    std::variant<SParseTerm, SParseRule> data;
};

//} //namespace huyacc

#endif //CPARSELEXEM_H
