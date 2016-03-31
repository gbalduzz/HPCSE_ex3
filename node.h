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

    Node():mass(0),xcom(0),ycom(0),child_id(-1),part_start(-1),part_end(-1){}

    int occupancy()const {return (part_start>=0 && part_end>=0) ? part_end-part_start+1 : 0;}
};

#endif //EX3_TREE_H
