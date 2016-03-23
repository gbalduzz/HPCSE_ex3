//
// Created by giovanni on 07.03.16.
//

#ifndef EX2_EXTENT_H
#define EX2_EXTENT_H

using pairf=std::pair<float,float>;

pairf MinMax(const float * const  x,int N)
{
    double min(x[0]),max(x[0]);
    for(int i=1;i<N;i++) {
        if (x[i] > max) max = x[i];
        else if (x[i] < min) min = x[i];
    }
    return pairf(min,max);
}

void extent(const int N, const float * const x,const float * const y,float& xmin,float& ymin,float& ext)
{
pairf x_mm=MinMax(x,N);
pairf y_mm=MinMax(y,N);
xmin=x_mm.first;
ymin=y_mm.first;
float  dx(x_mm.second-x_mm.first);
float  dy(y_mm.second-y_mm.first);
ext= dx>dy ? dx : dy;
}



#endif //EX2_EXTENT_H
