#include <iostream>
#include "build.h"
using std::cout; using std::endl;
void InitializeAtRandom(const int N,float* x,float* y,float* m);
void PrintInfo(const vector<Node>& );

int main()
{
    const int k=2;
    const int N=1000000;
    float* x=new float[N];
    float* y=new float[N];
    float* mass=new float[N];
    float* xsorted=new float[N];
    float* ysorted=new float[N];
    float* mass_sorted=new float[N];

    InitializeAtRandom(N,x,y,mass);
    vector<Node> tree=build(x,y,mass,N,k,xsorted,ysorted,mass_sorted);
    PrintInfo(tree);

    delete[] x; delete[] y; delete[] mass;
    delete[] xsorted; delete[] ysorted; delete[] mass_sorted;
}

#include <random>
void InitializeAtRandom(const int N,float* x,float* y,float* m)
{
    std::mt19937_64 mt(0);
    std::uniform_real_distribution<float> dist(0,1);
    for(int i=0;i<N;i++) {
        x[i]=dist(mt);
        y[i]=dist(mt);
        m[i]=1;
    }
}
void PrintInfo(const vector<Node>& tree){
    int i=0;
    for(auto& node : tree){
        cout<<"i="<<i<<" level: "<<node.level<<"\tfirst child: "<<node.child_id<<
        "\t N_points: "<<node.occupancy()<<
                "\tcom: "<<node.xcom<<" , "<<node.ycom<<endl;
        i++;
    }
}