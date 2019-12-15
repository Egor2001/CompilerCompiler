#ifndef STERM_H
#define STERM_H

#include <string>
#include <string_view>

//namespace huyacc {

struct STerm
{
    std::string_view name_view;
    std::string_view expr_view;
    std::string_view code_view;
};

//} //namespace huyacc

#endif //STERM_H
