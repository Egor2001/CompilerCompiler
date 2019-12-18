#ifndef SNODE_H
#define SNODE_H

//namespace {

enum ENode
{
    ...
};

struct SNode
{

    std::shared_ptr<SNode> left;
    std::shared_ptr<SNode> right;
};

//} //namespace

#endif //SNODE_H
