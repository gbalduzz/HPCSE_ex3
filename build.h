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
#include "find_last.h"

using std::vector;
omp_lock_t lock_tree;
void create_children_recursively(const int parent_id,vector<Node>&tree,const float* x,const float* y,const float* m,const uint* label,const int N,const int k);
void compute_com(vector<Node>& tree,float *x,float* y,float* m);
     
inline int check_size(const vector<Node>& tree);

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
    omp_init_lock(&lock_tree);
    {
        Profiler p("Tree creation");
        //create root tree[id]
        tree[0].level = 0;
        tree[0].morton_id = 0;
        tree[0].part_start = 0;
        tree[0].part_end = N - 1;

        create_children_recursively(0, tree,xsorted,ysorted,mass_sorted, label_ordered, N,k);
        compute_com(tree,xsorted,ysorted,mass_sorted);

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

void create_children_recursively(const int parent_id,vector<Node>&tree,const float* x,const float* y,const float* m,const uint* label,const int N,const int k) {
    static const int max_level = sizeof(int) * 4; //number of bits over 2
    if (tree[parent_id].occupancy() <= k) return;
    if (tree[parent_id].level == max_level) { //no more space for branching
        std::cout << "Warning: No more space for branching" << std::endl;
        tree[parent_id].child_id = -5;
        return;
    }
    uint mask = create_mask(tree[parent_id].level + 1);
    int first_child_id,start,end;

    //create enough space in the tree in a thread safe manner
    omp_set_lock(&lock_tree);
    first_child_id = tree.size();
    for (int i = 0; i < 4; i++) tree.push_back(Node());
    omp_unset_lock(&lock_tree);

    tree[parent_id].child_id = first_child_id;
    const int index_order[] = {0, 3, 1, 2};//traverse children in this order
    for (int i : index_order) {
        int child_id = first_child_id + i;
        tree[child_id].level = tree[parent_id].level + 1;
        tree[child_id].morton_id = get_new_id(tree[parent_id].morton_id, tree[child_id].level, i);
        //find points inside tree[id]
        switch (i) {
            case 0:
                tree[child_id].part_start = tree[parent_id].part_start;
                tree[child_id].part_end = find_last(tree[parent_id].part_start, tree[parent_id].part_end, label, mask,
                                                    tree[child_id].morton_id);
                break;

            case 3:
                tree[child_id].part_start = find_first(tree[parent_id].part_start, tree[parent_id].part_end, label,
                                                       mask, tree[child_id].morton_id);
                tree[child_id].part_end = tree[parent_id].part_end;
                break;
            case 1:
                start= tree[first_child_id].part_end==-1 ?  tree[parent_id].part_start : tree[first_child_id].part_end+1;
                tree[child_id].part_start = start;
                tree[child_id].part_end = find_last(start, tree[parent_id].part_end, label, mask,
                                                    tree[child_id].morton_id);
                break;
            default: //last one is determined by the others
                end=  tree[first_child_id+3].part_start==-1 ? tree[parent_id].part_end : tree[first_child_id+3].part_start-1;
                start=tree[first_child_id+1].part_end==-1   ?  tree[first_child_id].part_end==-1 ? tree[parent_id].part_start
                                                                                                     : tree[first_child_id].part_end+1
                                                                : tree[first_child_id+1].part_end+1;
                if(end>=start) {//not empty
                    tree[child_id].part_start = start;
                    tree[child_id].part_end = end;
                }
                break;
        }
    }
    for(int i=0;i<4;i++)//iterate on children
        create_children_recursively(first_child_id+i,tree,x,y,m,label,N,k);
}
//TODO apply same principle recursively
//TODO use find for part__end and part_start DONE?
void solve_dependencies(vector<Node>& tree,int id,float*x,float*y,float*m);

void compute_com(vector<Node>& tree,float *x,float* y,float* m){
    assert(tree.size()>=5); //at least one branching
    for(int i=1;i<5;i++){
        solve_dependencies(tree,i,x,y,m);
        tree[0].mass+=tree[i].mass;
        tree[0].xcom+=tree[i].mass*tree[i].xcom;
        tree[0].ycom+=tree[i].mass*tree[i].ycom;
    }
    tree[0].xcom/=tree[0].mass;
    tree[0].ycom/=tree[0].mass;
}

void solve_dependencies(vector<Node>& tree,int id,float*x,float*y,float*m){
    if(tree[id].child_id<0){
        if(! tree[id].occupancy()) return;
        for(int i=tree[id].part_start;i<=tree[id].part_end;i++){
            tree[id].mass+=m[i];
            tree[id].xcom+=m[i]*x[i];
            tree[id].ycom+=m[i]*y[i];
        }
    }
    else{
        int child_id;
        for(int i=0;i<4;i++){
            child_id=tree[id].child_id+i;
            solve_dependencies(tree,child_id,x,y,m);
            tree[id].mass+=tree[child_id].mass;
            tree[id].xcom+=tree[child_id].mass*tree[child_id].xcom;
            tree[id].ycom+=tree[child_id].mass*tree[child_id].ycom;
        }
    }
    tree[id].xcom/=tree[id].mass;
    tree[id].ycom/=tree[id].mass;
}
    

#endif //EX3_BUILD_H
