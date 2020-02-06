#include <cstdlib>
#include <cstdio>

#include <queue>
#include <string_view>

#include "CRegexNode.h"
#include "CENFAutomaton.h"
/*
#include "CRegex.h"
#include "CRegexParser.h"
*/
void print_regex(const std::unique_ptr<CRegexNode>& root_regex);
bool read_str(CENFAutomaton& enfa, std::string_view str);

int main()
{
/*
    CRegexParser parser;

    auto regex = parser.parse("biba|b{o[b|a]}");
    auto add_regex = CRegex("lol");
    regex.make_sum(std::move(add_regex));

    auto root = regex.root();
    print_regex(root);
*/
    auto root = CRegexNode::make_regex_cat(CRegexNode::make_regex_str('a'), 
                                           CRegexNode::make_regex_str('b')); 

    root = CRegexNode::make_regex_sum(CRegexNode::copy_regex(root), 
                                      CRegexNode::make_regex_str('c'));

    print_regex(root);

    CENFAutomaton enfa = 
        make_cat_enfa(
            make_sum_enfa(
                make_opt_enfa(
                    make_str_enfa('a')
                    ),
                make_rep_enfa(
                    make_str_enfa('b')
                    )
                ),
            make_rep_enfa(
                make_str_enfa('c')
                )
            );

    bool is_accepted = read_str(enfa, "accc");

    printf("\nis_accepted = %s", (is_accepted ? "true" : "false"));
    printf("\nlist size = %zu", enfa.node_list().size());

    return 0;
}

bool read_str(CENFAutomaton& enfa, std::string_view str)
{
    bool result = true;

    std::unordered_set<CENFAutomaton::TIter, 
                       CENFAutomaton::SNodeListItHash> old_set;
    old_set = enfa.next_enfa_eps(enfa.start_it());

    std::unordered_set<CENFAutomaton::TIter, 
                       CENFAutomaton::SNodeListItHash> new_set;
    for (char symb : str)
    {
        for (const auto& it : old_set)
        {
            auto it_set = enfa.next_enfa_set(it, symb);
            for (const auto& sub_it : it_set)
                new_set.merge(enfa.next_enfa_eps(sub_it));
        }

        old_set = std::move(new_set);
        new_set.clear();
    }

    result = (old_set.count(enfa.accepting_it()));

    return result;
}

void print_regex(const std::unique_ptr<CRegexNode>& root_regex)
{
    if (root_regex)
    {
        switch (root_regex->type())
        {
            case REGEX_SUM:
                printf("(");
                print_regex(root_regex->left());
                printf(")|(");
                print_regex(root_regex->right());
                printf(")");
                break;

            case REGEX_CAT:
                print_regex(root_regex->left());
                print_regex(root_regex->right());
                break;

            case REGEX_OPT:
                printf("[");
                print_regex(root_regex->left());
                printf("]");
                break;

            case REGEX_REP:
                printf("{");
                print_regex(root_regex->left());
                printf("}");
                break;

            case REGEX_STR:
                printf("%c", static_cast<char>(root_regex->data()));
                break;

            case REGEX_DEL:
                break;

            default:
                break;
        }
    }
}
