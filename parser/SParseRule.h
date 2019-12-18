#ifndef SPARSERULE_H
#define SPARSERULE_H

#include <string_view>

//namespace huyacc {

struct SParseRule
{
    std::string_view name_view;
    std::string_view expr_view;
    std::string_view code_view;
};

//} //namespace huyacc

#endif //SPARSERULE_H
