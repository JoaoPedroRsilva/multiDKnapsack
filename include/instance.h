#ifndef INSTANCE_H
#define INSTANCE_H
#include <vector>
#include <string>

struct Instance{
    int id;
    int n;
    int m;
    double optimalValue;
    std::vector<double> profits;
    std::vector<std::vector<double>> constraints;
    std::vector<double> capacities;

    void printSummary() const;
};

std::vector<Instance> readInstances( const std::string& filepath );

#endif
