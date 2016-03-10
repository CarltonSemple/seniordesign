#ifndef UTIL_H
#define UTIL_H

#include <tuple>
#include <cmath>

class Util
{
    public:
    static double distance(std::pair<double,double> p1, std::pair<double,double> p2)
    {
        double m = pow(p1.first - p2.first, 2) + pow(p2.second - p2.second, 2);
        double s = sqrt(m);
        return s;
    }  
};

#endif