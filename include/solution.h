#ifndef SOLUTION_H
#define SOLUTION_H
#include "instance.h"
#include <vector>

struct Solution{
    std::vector<bool> x;
    double totalProfit;
    std::vector<double> resourceUsage;
    bool isValid;

    Solution();
    Solution( int n );

    void evaluate( const Instance& inst );
    void repair( const Instance& inst );
    void generateNeighbor( const Instance& inst );
    void print( const Instance& inst ) const;
};

#endif
