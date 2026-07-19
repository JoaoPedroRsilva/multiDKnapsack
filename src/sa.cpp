#include "../include/sa.h"
#include <cmath>
#include <random>

static std::mt19937 rng( std::random_device{}() );

SimulatedAnnealing::SimulatedAnnealing( double t0, double alpha, double tMin, int L ){
    this->initialTemp = t0;
    this->coolingRate = alpha;
    this->finalTemp = tMin;
    this->iterationsPerTemp = L;
}

Solution SimulatedAnnealing::run( const Instance& inst ){
    Solution currentSol( inst.n );
    currentSol.repair( inst );
    
    Solution bestSol = currentSol;
    double T = this->initialTemp;
    
    std::uniform_real_distribution<double> dist( 0.0, 1.0 );

    while( T > this->finalTemp ){
        for( int step = 0; step < this->iterationsPerTemp; step++ ){
            Solution newSol = currentSol;
            newSol.generateNeighbor( inst );

            double deltaE = newSol.totalProfit - currentSol.totalProfit;

            if( deltaE > 0 ){
                currentSol = newSol;
                if( currentSol.totalProfit > bestSol.totalProfit ){
                    bestSol = currentSol;
                }
            } else {
                double p = std::exp( deltaE / T );
                if( dist( rng ) < p ){
                    currentSol = newSol;
                }
            }
        }
        
        T *= this->coolingRate;
    }

    return bestSol;
}
