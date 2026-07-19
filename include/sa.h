#ifndef SA_H
#define SA_H
#include "instance.h"
#include "solution.h"

class SimulatedAnnealing{
private:
    double initialTemp;
    double coolingRate;
    double finalTemp;
    int iterationsPerTemp;

public:
    SimulatedAnnealing( double t0, double alpha, double tMin, int L );
    Solution run( const Instance& inst );
};

#endif
