#ifndef SLEXEM_H
#define SLEXEM_H

#include <type_traits>
#include <variant>
#include <vector>
#include <memory>

//namespace {

struct SLexem;

struct SLexTerm
{
    explicit SLexTerm(const std::string_view& name_view_set):
        name_view(name_view_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::string_view name_view;
};

struct SLexRule
{
    explicit SLexRule(const std::string_view& name_view_set):
        name_view(name_view_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::string_view name_view;
};

struct SLexCat
{
    SLexCat(const std::shared_ptr<SLexem>& left_set,
            const std::shared_ptr<SLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::shared_ptr<SLexem> left; 
    std::shared_ptr<SLexem> right; 
};

struct SLexXor
{
    SLexXor(const std::shared_ptr<SLexem>& left_set,
            const std::shared_ptr<SLexem>& right_set):
        left(left_set),
        right(right_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::shared_ptr<SLexem> left; 
    std::shared_ptr<SLexem> right; 
};

struct SLexRep
{
    explicit SLexRep(const std::shared_ptr<SLexem>& left_set):
        left(left_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::shared_ptr<SLexem> left; 
};

struct SLexOpt
{
    explicit SLexOpt(const std::shared_ptr<SLexem>& left_set):
        left(left_set)
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const;

    std::shared_ptr<SLexem> left; 
};

struct SLexem
{
    template<typename TData>
    explicit SLexem(const TData& data_set):
       data(data_set) 
    {}

    template<typename TStream>
    void build_parse(TStream& out_stream) const
    {
        auto visitor = [&out_stream](const auto& lex) 
        { 
            using lex_t = std::remove_cv_t<
                              std::remove_reference_t<
                                  decltype(lex)
                                  >
                              >;

            if constexpr (!std::is_same_v<lex_t, std::monostate>)
                lex.build_parse(out_stream); 
        };

        std::visit(visitor, data);
    }

    std::variant<std::monostate, SLexTerm, SLexRule,
                 SLexCat, SLexXor, 
                 SLexRep, SLexOpt> data;
};

template<typename TStream>
void SLexTerm::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexTerm(";
    out_stream << "TERM_" << name_view;
    out_stream << "))";
}

template<typename TStream>
void SLexRule::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexRule(";
    out_stream << "RULE_" << name_view;
    out_stream << "))";
}

template<typename TStream>
void SLexCat::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexCat(";

    if (left) left->build_parse(out_stream);
    else      out_stream << "nullptr";

    out_stream << ", ";

    if (right) right->build_parse(out_stream);
    else       out_stream << "nullptr";

    out_stream << "))";
}

template<typename TStream>
void SLexXor::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexXor(";

    if (left) left->build_parse(out_stream);
    else      out_stream << "nullptr";

    out_stream << ", ";

    if (right) right->build_parse(out_stream);
    else       out_stream << "nullptr";

    out_stream << "))";
}

template<typename TStream>
void SLexRep::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexRep(";

    if (left) left->build_parse(out_stream);
    else      out_stream << "nullptr";

    out_stream << "))";
}

template<typename TStream>
void SLexOpt::build_parse(TStream& out_stream) const
{
    out_stream << "std::make_shared<SHuyaccLexem>(SHuyaccLexOpt(";

    if (left) left->build_parse(out_stream);
    else      out_stream << "nullptr";

    out_stream << "))";
}

//} //namespace

#endif //SLEXEM_H
