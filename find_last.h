//
// Created by giovanni on 30.03.16.
//
#ifndef EX3_FIND_LAST_H
#define EX3_FIND_LAST_H
#include<assert.h>
#include<iostream>

int find_last(int start,int end,const uint* const  label,const uint mask,const uint id){ //find last label that satisfies equality up to mask
    assert(end>=start);
    //assume start is correct start of interval
    if( (label[start] & mask) != id) return -1; //no hit
    int try_at;
    while(end-start>1){
     try_at=(end+start)/2;
        if( (label[try_at] & mask) == id) start=try_at;
        else end=try_at;
 }
    if( (label[end] & mask) == id) return end;
    else  return start;
}

int find_first(int start,int end,const uint* const  label,const uint mask,const uint id){ //find first label that satisfies equality up to mask
    assert(end>=start);
    //assume end is correct end of interval
    if( (label[end] & mask) != id) return -1; //no hit
    int try_at;
    while(end-start>1){
        try_at=(end+start)/2;
        if( (label[try_at] & mask) !=  id) start=try_at;
        else end=try_at;
    }
    if( (label[start] & mask) == id) return start;
    else return end;
}

#endif //EX3_FIND_LAST_H
