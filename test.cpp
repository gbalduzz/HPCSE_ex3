//
// Created by giovanni on 23.03.16.
//
#include "build.h"
#include <bitset>
#include <iostream>
using namespace std;
int main()
{
    cout<<"mask of 0 is "<<bitset<32>(create_mask(0))<<endl;
    cout<<"mask of 2 is "<<bitset<32>(create_mask(2))<<endl;
    cout<<"mask of 3 is "<<bitset<32>(create_mask(3))<<endl;
    cout<<endl;
    uint parent=create_mask(1);
    cout<<"first child is "<<bitset<32>(get_new_id(parent,2,0))<<endl;
    cout<<"second child is "<<bitset<32>(get_new_id(parent,2,1))<<endl;
    cout<<"fourth child is "<<bitset<32>(get_new_id(parent,2,3))<<endl;
    cout<<endl;
    cout<<"masked is "<<bitset<32>(get_new_id(parent,2,3) & create_mask(1))<<endl;
}