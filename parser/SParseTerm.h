#ifndef SPARSETERM_H
#define SPARSETERM_H

#include <string_view>

//namespace huyacc {

struct SParseTerm
{
    std::string_view name_view;
    std::string_view expr_view;
    std::string_view code_view;
};

//} //namespace huyacc

#endif //SPARSETERM_H
