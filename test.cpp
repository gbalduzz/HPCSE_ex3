//
// Created by giovanni on 23.03.16.
//
#include "build.h"
#include <bitset>
#include <iostream>
using namespace std;
int main()
{
    const int N=10;
    uint v[N]={3,3,3,3,3,5,5,5,5,5};
    uint mask=-1;
    int l=find_last(0,9,v,mask,3);
    int f=find_first(0,9,v,mask,5);
    cout<<"last 3 hit= "<<l<<endl;
    cout<<"first 5  hit= "<<f<<endl;

    uint v2[N]={0,0,0,0,0,0,0,0,0,5};
    l=find_last(0,9,v2,mask,3);
    f=find_first(0,9,v2,mask,5);
    cout<<"last v2= "<<l<<endl;
    cout<<"first v2= "<<f<<endl;
}