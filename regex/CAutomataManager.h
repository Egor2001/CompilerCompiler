#ifndef CAUTOMATAMANAGER_H
#define CAUTOMATAMANAGER_H

#include "CENFAutomaton.h"
#include "CDFAutomaton.h"

//namespace {

CDFAutomaton powerset(const CENFAutomaton&);
CENFAutomaton inverse(const CENFAutomaton&);
CENFAutomaton convert(const CDFAutomaton&);
CDFAutomaton minimize(const CDFAutomaton&);

CDFAutomaton powerset(const CENFAutomaton& enfa)
{
    CDFAutomaton result;

    return result;
}

CENFAutomaton inverse(const CENFAutomaton& enfa)
{
    CENFAutomaton result;

    return result;
}

CENFAutomaton convert(const CDFAutomaton& dfa)
{
    CENFAutomaton result;

    return result;
}

CDFAutomaton minimize(const CDFAutomaton&)
{
    CENFAutomaton result;

    return result;
}

//} //namespace

#endif //CAUTOMATAMANAGER_H
