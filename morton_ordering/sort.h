//
// Created by giovanni on 08.03.16.
//
#ifndef HPCSE2_EX2_SORT_H
#define HPCSE2_EX2_SORT_H
#include<algorithm>
using std::pair;

void sort(const int N,uint* index,uint* keys)
{
    pair<uint,uint> *p=new pair<uint,uint>[N];
    for(int i=0;i<N;i++) {
        p[i].first = index[i];
        p[i].second= i;
    }
    //order the keys according to the relation between indexes
    std::sort(p, p+N);

    for(int i=0;i<N;i++){
        index[i]=p[i].first;
        keys[i]=p[i].second;
    }
    delete[] p;
}

#endif //HPCSE2_EX2_SORT_H
