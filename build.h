//
// Created by giovanni on 23.03.16.
//

#ifndef EX3_BUILD_H
#define EX3_BUILD_H

#include <vector>
#include <assert.h>
#include "node.h"
#include "morton_ordering/morton_includes.inc"
#include "profiler.h"
using std::vector;

void create_children(const Node& parent, vector<Node>& tree,uint* m_index,const int N);

vector<Node> build(const float* const x, const float* const y,
const float* mass,const int N,const int k, float* xsorted,float*ysorted,float* mass_sorted)
{
    //perform the exrcise 2 and find the morton labels of the points
    int* keys=new int[N];
    uint* m_label=new uint[N];
    uint* m_label_ordered=new uint[N];
    float xmin,ymin,ext;
    extent(N,x,y,xmin,ymin,ext);
    morton(N,x,y,xmin,ymin,ext,m_label);
    sort(N,m_label,keys);
    reorder(N,keys,x,y,mass,m_label,xsorted,ysorted,mass_sorted,m_label_ordered);
    delete[] keys; delete[] m_label;

    vector<Node> tree(1);
    {
        Profiler p("Tree creation");
        //create root node
        tree[0].level = 0;
        tree[0].morton_id = 0;
        tree[0].part_end = 0;
        tree[0].part_end = N - 1;

        const int max_level = sizeof(int) * 4; //number of bits over 2
        //if number of particles > k  : split
        for (int i = 0; i < tree.size(); i++) {
            if (tree[i].occupancy() > k) {
                if (tree[i].level == max_level) { //no more space for branching
                    tree[i].child_id = -5;
                    continue;
                }
                tree[i].child_id = tree.size();
                create_children(tree[i], tree, m_label_ordered, N);
            }
        }

    }//end Profiler

    delete[] m_label_ordered;
    return tree;
}

inline int get_new_id(uint parent_id,int level,int i)
{
static const uint n_bits=sizeof(int)*8;
    assert(2*level < n_bits);
    return parent_id  | i<< (n_bits-2*level);
}

uint create_mask(int level)
//create a mask that ignores the bits in the morton index corresponding to a higher level
// the mask is 2*level 1s followed by 0s
{
    static const uint n_bits=sizeof(int)*8;
    return ((1 << 2*level)-1) << (n_bits-2*level);
}

void create_children(const Node& parent, vector<Node>& tree,uint* m_index,const int N)
{
    int current_idx=parent.part_start;
    uint mask=create_mask(parent.level+1);
   for(int i=0;i<4;i++) {
       tree.push_back(Node());
       Node* child=&tree.back();
       child->level=parent.level+1;
       child->morton_id=get_new_id(parent.morton_id,child->level,i);
       //find points inside node
       if((m_index[current_idx] & mask) == child->morton_id){//branch is not empty
           child->part_start=current_idx;
           while((m_index[current_idx] & mask) == child->morton_id
                 &&  current_idx<N) current_idx++;
           child->part_end=current_idx-1;
       }
       else{//is empty
           child->part_start=-1;
           child->part_end=-2;
       }
   }

}
#endif //EX3_BUILD_H
