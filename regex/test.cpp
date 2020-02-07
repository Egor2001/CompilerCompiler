#include <cstdlib>
#include <cstdio>

#include <queue>
#include <string_view>

#include "CENFAutomaton.h"
#include "CDFAutomaton.h"

//bool read_str(CENFAutomaton& enfa, std::string_view str);

int main()
{
    CENFAutomaton<char> enfa = 
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

    bool is_accepted = enfa_read_str(enfa, "accc");

    printf("\nis_accepted = %s", (is_accepted ? "true" : "false"));
    printf("\nlist size = %zu", enfa.node_list().size());

    return 0;
}
/*
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
*/
