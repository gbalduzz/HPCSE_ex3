#include <iostream>
#include "build.h"
using std::cout; using std::endl;
void InitializeAtRandom(const int N,float* x,float* y,float* m);
void PrintInfo(const vector<Node>& );

int main()
{
    const int k=1;
    const int N=4;
    float x[]={0.1,0.75,0.2,0.75};
    float y[]={0.2,0.25,0.8,0.9};
    float mass[]={1,1,1,1};
    float* xsorted=new float[N];
    float* ysorted=new float[N];
    float* mass_sorted=new float[N];

    vector<Node> tree=build(x,y,mass,N,k,xsorted,ysorted,mass_sorted);
    PrintInfo(tree);

    delete[] xsorted; delete[] ysorted; delete[] mass_sorted;
}

/*#include <random>
void InitializeAtRandom(const int N,float* x,float* y,float* m)
{
    std::mt19937_64 mt(0);
    std::uniform_real_distribution<float> dist(0,1);
    for(int i=0;i<N;i++) {
        x[i]=dist(mt);
        y[i]=dist(mt);
        m[i]=1;
    }
}*/

void PrintInfo(const vector<Node>& tree){
    int i=0;
    for(auto& node : tree){
        cout<<"i="<<i<<" level: "<<node.level<<"\tfirst child: "<<node.child_id<<
        "\t N_points: "<<node.occupancy()<<endl;
        i++;
    }
}