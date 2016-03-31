#include <iostream>
#include<omp.h>
#include "build.h"
using std::cout; using std::endl;
void InitializeAtRandom(const int N,float* x,float* y,float* m);
void PrintInfo(const vector<Node>& ,int n_print=-1);

int main()
{
    const int k=2;
    const int N=10;
    const int n_print=10;
    float* x=new float[N];
    float* y=new float[N];
    float* mass=new float[N];
    float* xsorted=new float[N];
    float* ysorted=new float[N];
    float* mass_sorted=new float[N];

    //omp_set_max_active_levels(2);
    /**/omp_set_num_threads(1);
    cout<<"parallelizing over "<<omp_get_max_threads()<<" threads"<<endl<<endl;

    InitializeAtRandom(N,x,y,mass);
    vector<Node> tree=build(x,y,mass,N,k,xsorted,ysorted,mass_sorted);
    PrintInfo(tree,n_print);

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

void PrintInfo(const vector<Node>& tree,int n_print){
    cout<<endl;
    n_print=n_print>0 ?  std::min((int)tree.size(),n_print) : tree.size();
    for(int i=tree.size()-n_print;i<tree.size();i++){
        cout<<"i="<<i<<" level: "<<tree[i].level<<"\tfirst child: "<<tree[i].child_id<<
        "\t N_points: "<<tree[i].occupancy()<<
                "\tcom: "<<tree[i].xcom<<" , "<<tree[i].ycom<<endl;
    }
}