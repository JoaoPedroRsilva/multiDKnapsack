#include "../include/instance.h"
#include <iostream>
#include <fstream>
#include <cctype>

void Instance::printSummary() const{
    std::cout << "Instancia ID: " << id << "\n"
              << "  Itens (n): " << n << "\n"
              << "  Recursos (m): " << m << "\n"
              << "  Valor Otimo Teorico: " << optimalValue << "\n";
    std::cout << "  Capacidades de Recursos: [";
    for( int i = 0; i < m; i++ ){
        std::cout << capacities[i] << ( i == m - 1 ? "" : ", " );
    }
    std::cout << "]\n";
}

std::vector<Instance> readInstances( const std::string& filepath ){
    std::vector<Instance> instances;
    std::ifstream infile( filepath );
    
    if( !infile.is_open() ){
        std::cerr << "Erro ao abrir o arquivo: " << filepath << "\n";
        return instances;
    }

    int K = 0;
    if( !( infile >> K ) ){
        std::cerr << "Erro ao ler a quantidade de instancias (K).\n";
        return instances;
    }

    for( int k = 0; k < K; k++ ){
        Instance inst;
        inst.id = k + 1;
        
        if( !( infile >> inst.n >> inst.m >> inst.optimalValue ) ){
            break; 
        }

        // 1. Lucros (n valores)
        inst.profits.resize( inst.n );
        for( int j = 0; j < inst.n; j++ ){
            if( !( infile >> inst.profits[j] ) ){
                std::cerr << "Erro ao ler lucros da instancia " << inst.id << "\n";
                return instances;
            }
        }

        // 2. Matriz de restricoes (m x n)
        inst.constraints.assign( inst.m, std::vector<double>( inst.n, 0.0 ) );
        for( int i = 0; i < inst.m; i++ ){
            for( int j = 0; j < inst.n; j++ ){
                if( !( infile >> inst.constraints[i][j] ) ){
                    std::cerr << "Erro ao ler restricoes da instancia " << inst.id << "\n";
                    return instances;
                }
            }
        }

        // 3. Capacidades (m valores)
        inst.capacities.resize( inst.m );
        for( int i = 0; i < inst.m; i++ ){
            if( !( infile >> inst.capacities[i] ) ){
                std::cerr << "Erro ao ler capacidades da instancia " << inst.id << "\n";
                return instances;
            }
        }

        instances.push_back( inst );
    }

    infile.close();
    return instances;
}
