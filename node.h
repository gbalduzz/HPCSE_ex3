//
// Created by giovanni on 23.03.16.
//

#ifndef EX3_TREE_H
#define EX3_TREE_H
using uint= unsigned int;

struct Node{
    int level;
    uint morton_id;
    int child_id;

    int part_start,part_end;
    float mass,xcom,ycom;
};

#endif //EX3_TREE_H
