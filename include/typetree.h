#ifndef TYPETREE_H
#define TYPETREE_H

#include <string>
#include <vector>

typedef struct typetree
{
    std::string name;
    std::string type;
    unsigned int bytes;
    unsigned int byte_start;
    //std::vector<typetree> children;
}typetree;

#endif
