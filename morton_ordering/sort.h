//
// Created by giovanni on 08.03.16.
//
#ifndef HPCSE2_EX2_SORT_H
#define HPCSE2_EX2_SORT_H
#include<algorithm>

void sort(const int N,uint* index,int* keys)
{
    #pragma omp parallel for schedule(static)
    for(int i=0;i<N;i++) keys[i]=i;
    //order the keys according to the relation between indexes
    __gnu_parallel::sort(keys, keys+N,
         [&](const int& a, const int& b) {
             return (index[a] < index[b]);
         }
    );
}

#endif //HPCSE2_EX2_SORT_H
