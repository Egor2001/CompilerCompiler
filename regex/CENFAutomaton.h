#ifndef CENFAUTOMATON_H
#define CENFAUTOMATON_H

#include <cstdlib>
#include <cstring>
#include <list>

#include "CRegex.h"

//namespace {

//TODO: to refactor with template symbol type
//TODO: to refactor with persistent state list
//TODO: noexcept
class CENFAutomaton
{
public:
    //TODO: to replace map with list of transitions 
    //the first state is always the head of state list
    struct SENFAState
    {
        bool is_accepting;
        
        std::unordered_map<char, std::list<SENFAState>::iterator> 
            transition_map;
    };

public:
    explicit CENFAutomaton(const CRegex& regex);

    CENFAutomaton             (const CENFAutomaton&);
    CENFAutomaton& operator = (const CENFAutomaton&);
    CENFAutomaton             (CENFAutomaton&&);
    CENFAutomaton& operator = (CENFAutomaton&&);

    ~CENFAutomaton();

    CRegex create_regex() const;
    CDFAutomaton create_dfa() const;

private:
    //head is always the first state
    std::list<SENFAState> state_list_;
};

//} //namespace 

#endif //CENFAUTOMATON_H
