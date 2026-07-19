#include "../include/solution.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

static std::mt19937 rng( std::random_device{}() );

Solution::Solution(){
    this->totalProfit = 0.0;
    this->isValid = true;
}

Solution::Solution( int n ){
    this->x.assign( n, false );
    this->totalProfit = 0.0;
    this->resourceUsage.assign( 0, 0.0 );
    this->isValid = true;
}

void Solution::evaluate( const Instance& inst ){
    this->totalProfit = 0.0;
    this->resourceUsage.assign( inst.m, 0.0 );
    this->isValid = true;

    for( int j = 0; j < inst.n; j++ ){
        if( this->x[j] ){
            this->totalProfit += inst.profits[j];
            for( int i = 0; i < inst.m; i++ ){
                this->resourceUsage[i] += inst.constraints[i][j];
            }
        }
    }

    for( int i = 0; i < inst.m; i++ ){
        if( this->resourceUsage[i] > inst.capacities[i] ){
            this->isValid = false;
            break;
        }
    }
}

void Solution::repair( const Instance& inst ){
    this->evaluate( inst );
    if( this->isValid ){
        return;
    }

    std::vector<int> selectedItems;
    for( int j = 0; j < inst.n; j++ ){
        if( this->x[j] ){
            selectedItems.push_back( j );
        }
    }

    std::vector<double> ratios( inst.n, 0.0 );
    for( int j : selectedItems ){
        double resourceSum = 0.0;
        for( int i = 0; i < inst.m; i++ ){
            if( inst.capacities[i] > 0 ){
                resourceSum += inst.constraints[i][j] / inst.capacities[i];
            } else {
                resourceSum += inst.constraints[i][j];
            }
        }
        ratios[j] = inst.profits[j] / ( resourceSum + 1e-9 );
    }

    std::sort( selectedItems.begin(), selectedItems.end(), [&]( int a, int b ){
        return ratios[a] < ratios[b];
    } );

    for( int j : selectedItems ){
        this->x[j] = false;
        this->totalProfit -= inst.profits[j];
        
        bool currentValid = true;
        for( int i = 0; i < inst.m; i++ ){
            this->resourceUsage[i] -= inst.constraints[i][j];
            if( this->resourceUsage[i] > inst.capacities[i] ){
                currentValid = false;
            }
        }

        if( currentValid ){
            this->isValid = true;
            break;
        }
    }
}

void Solution::generateNeighbor( const Instance& inst ){
    if( inst.n <= 0 ){
        return;
    }

    int j = rng() % inst.n;
    this->x[j] = !this->x[j];

    this->repair( inst );
}

void Solution::print( const Instance& inst ) const{
    std::cout << "Lucro Total: " << this->totalProfit << "\n";
    std::cout << "Valida: " << ( this->isValid ? "Sim" : "Nao" ) << "\n";
    std::cout << "Recursos Utilizados:\n";
    for( int i = 0; i < inst.m; i++ ){
        double pct = ( inst.capacities[i] > 0 ) ? ( this->resourceUsage[i] / inst.capacities[i] ) * 100.0 : 0.0;
        std::cout << "  Recurso " << i + 1 << ": " << this->resourceUsage[i] 
                  << " / " << inst.capacities[i] << " (" << pct << "%)\n";
    }
    std::cout << "Itens Selecionados: [ ";
    for( int j = 0; j < inst.n; j++ ){
        if( this->x[j] ){
            std::cout << j + 1 << " ";
        }
    }
    std::cout << "]\n";
}
