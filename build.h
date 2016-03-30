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
void create_children(const int parent_id, vector<Node>& tree,const float* x,const float* y,const float* m,const uint* label,const int N);

vector<Node> build(const float* const x, const float* const y,
const float* mass,const int N,const int k, float* xsorted,float*ysorted,float* mass_sorted)
{
    //perform the exrcise 2 and find the morton labels of the points
        int *keys = new int[N];
        uint *label = new uint[N];
        uint *label_ordered = new uint[N];
        float xmin, ymin, ext;
    {
        Profiler p("old morton labelling");
        extent(N, x, y, xmin, ymin, ext);
        morton(N, x, y, xmin, ymin, ext, label);
        sort(N, label, keys);
        reorder(N, keys, x, y, mass, label, xsorted, ysorted, mass_sorted, label_ordered);
    }
    delete[] keys; delete[] label;

    vector<Node> tree(1);
    tree.reserve(N*2);
    {
        Profiler p("Tree creation");
        //create root node
        tree[0].level = 0;
        tree[0].morton_id = 0;
        tree[0].part_start = 0;
        tree[0].part_end = N - 1;

        const int max_level = sizeof(int) * 4; //number of bits over 2
        //if number of particles > k  : split
        for (int i = 0; i < tree.size(); i++) {
            if (tree[i].occupancy() > k) {
                if (tree[i].level == max_level) { //no more space for branching
                    std::cout<<"Warning: No more space for branching"<<std::endl;
                    tree[i].child_id=-5;
                    continue;
                }
                tree[i].child_id = tree.size();
                create_children(i, tree,xsorted,ysorted,mass_sorted, label_ordered, N);
            }
        }

    }//end Profiler

    delete[] label_ordered;
    return tree;
}

inline int get_new_id(uint parent_id,int level,int i)
{
static const uint n_bits=sizeof(int)*8;
    assert(2*level <= n_bits);
    return parent_id  | i<< (n_bits-2*level);
}

uint create_mask(int level)
//create a mask that ignores the bits in the morton index corresponding to a higher level
// the mask is 2*level 1s followed by 0s
{
    static const uint n_bits=sizeof(int)*8;
    assert(2*level <= n_bits);
    return 2*level==n_bits ? -1 : //otherwise returns all 0 instead of all 1
            ((1 << 2*level)-1) << (n_bits-2*level);
}

void create_children(const int parent_id, vector<Node>& tree,
                     const float* x,const float* y,const float* m,const uint* label,const int N)
{
    int current_idx=tree[parent_id].part_start;
    uint mask=create_mask(tree[parent_id].level+1);
    for(int i=0;i<4;i++) {
       tree.push_back(Node());
       int child_id=tree.size()-1;
       tree[child_id].level=tree[parent_id].level+1;
       tree[child_id].morton_id=get_new_id(tree[parent_id].morton_id,tree[child_id].level,i);
       //find points inside node
       if((label[current_idx] & mask) == tree[child_id].morton_id){//branch is not empty
           tree[child_id].part_start=current_idx;
           while((label[current_idx] & mask) == tree[child_id].morton_id
                 &&  current_idx<N) {
               tree[child_id].xcom+=m[current_idx]*x[current_idx];
               tree[child_id].ycom+=m[current_idx]*y[current_idx];
               tree[child_id].mass+=m[current_idx];
               current_idx++;
           }
           tree[child_id].part_end=current_idx-1;
           tree[child_id].xcom/=tree[child_id].mass;
           tree[child_id].ycom/=tree[child_id].mass;
       }
       else{//is empty
           tree[child_id].part_start=-1;
           tree[child_id].part_end=-2;
       }
   }
}

#endif //EX3_BUILD_H
