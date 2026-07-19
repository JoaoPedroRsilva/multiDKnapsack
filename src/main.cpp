#include "../include/instance.h"
#include "../include/solution.h"
#include "../include/sa.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>

int getMenuChoice( int minVal, int maxVal ){
    int choice;
    while( true ){
        std::cout << "Escolha uma opcao (" << minVal << "-" << maxVal << "): ";
        if( std::cin >> choice && choice >= minVal && choice <= maxVal ){
            return choice;
        }
        std::cin.clear();
        std::cin.ignore( 32767, '\n' );
        std::cout << "Opcao invalida. Tente novamente.\n";
    }
}

void runSingleInstance( const Instance& inst, double t0, double alpha, double tMin, int L ){
    std::cout << "\n=============================================\n";
    std::cout << "Executando Instancia ID: " << inst.id << " (n=" << inst.n << ", m=" << inst.m << ")\n";
    std::cout << "=============================================\n";

    auto startTime = std::chrono::high_resolution_clock::now();

    SimulatedAnnealing sa( t0, alpha, tMin, L );
    Solution bestSol = sa.run( inst );

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;

    bestSol.print( inst );

    std::cout << "\n---------------- CORRETUDE ----------------\n";
    if( bestSol.isValid ){
        std::cout << "Solucao VIAVEL (Nenhuma capacidade de recurso foi estourada).\n";
    } else {
        std::cout << "AVISO: Solucao INVIABEL (Estouro de recursos!)\n";
    }

    if( inst.optimalValue > 0 ){
        double gap = ( ( inst.optimalValue - bestSol.totalProfit ) / inst.optimalValue ) * 100.0;
        double quality = ( bestSol.totalProfit / inst.optimalValue ) * 100.0;
        std::cout << "Valor Otimo Teorico: " << inst.optimalValue << "\n";
        std::cout << "Qualidade da Solucao: " << std::fixed << std::setprecision( 2 ) << quality << "%\n";
        std::cout << "GAP (Distancia do Otimo): " << gap << "%\n";
    } else {
        std::cout << "Valor Otimo Teorico: Desconhecido (0)\n";
    }

    std::cout << "Tempo de Execucao: " << std::fixed << std::setprecision( 2 ) << duration.count() << " ms\n";
}

void runBatchInstances( const std::vector<Instance>& instances, double t0, double alpha, double tMin, int L ){
    std::cout << "\n=============================================\n";
    std::cout << "Executando todas as " << instances.size() << " instancias em lote (Batch Mode)\n";
    std::cout << "=============================================\n";

    struct ResultRow{
        int id;
        int n;
        int m;
        double opt;
        double found;
        double quality;
        double durationMs;
        bool valid;
    };

    std::vector<ResultRow> results;

    for( const auto& inst : instances ){
        std::cout << "Rodando instancia " << inst.id << " / " << instances.size() << "..." << std::flush;
        
        auto start = std::chrono::high_resolution_clock::now();
        SimulatedAnnealing sa( t0, alpha, tMin, L );
        Solution bestSol = sa.run( inst );
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;

        double quality = ( inst.optimalValue > 0 ) ? ( bestSol.totalProfit / inst.optimalValue ) * 100.0 : 0.0;

        results.push_back( {
            inst.id,
            inst.n,
            inst.m,
            inst.optimalValue,
            bestSol.totalProfit,
            quality,
            duration.count(),
            bestSol.isValid
        } );
        std::cout << " Concluido (" << duration.count() << " ms)\n";
    }

    std::cout << "\n### TABELA DE RESULTADOS (Pronta para o Relatorio)\n\n";
    std::cout << "| Inst. |  n  |  m  | Opt. Teorico | Lucro Encontrado | Qualidade (%) | Tempo (ms) | Viavel |\n";
    std::cout << "|-------|-----|-----|--------------|------------------|---------------|------------|--------|\n";
    
    double sumQuality = 0.0;
    double sumTime = 0.0;
    int optCount = 0;
    int instancesWithOpt = 0;

    for( const auto& r : results ){
        std::cout << "| " << std::setw( 5 ) << r.id
                  << " | " << std::setw( 3 ) << r.n
                  << " | " << std::setw( 3 ) << r.m;
        
        std::cout << std::fixed;
        
        if( r.opt > 0 ){
            std::cout << " | " << std::setw( 12 ) << std::setprecision( 1 ) << r.opt
                      << " | " << std::setw( 16 ) << std::setprecision( 1 ) << r.found
                      << " | " << std::setw( 12 ) << std::setprecision( 2 ) << r.quality << "%";
            
            sumQuality += r.quality;
            instancesWithOpt++;
            if( r.found >= r.opt ){
                optCount++;
            }
        } else {
            std::cout << " | " << std::setw( 12 ) << "N/A"
                      << " | " << std::setw( 16 ) << std::setprecision( 1 ) << r.found
                      << " | " << std::setw( 13 ) << "N/A";
        }
        
        std::cout << " | " << std::setw( 9 ) << std::setprecision( 1 ) << r.durationMs
                  << " | " << ( r.valid ? "Sim" : "Nao" ) << " |\n";
        
        sumTime += r.durationMs;
    }
    
    std::cout << "\n### Resumo Estatistico:\n";
    if( instancesWithOpt > 0 ){
        std::cout << "- Qualidade Media: " << std::fixed << std::setprecision( 2 ) << ( sumQuality / instancesWithOpt ) << "%\n";
        std::cout << "- Vezes que atingiu o Otimo: " << optCount << " de " << instancesWithOpt << "\n";
    } else {
        std::cout << "- Qualidade Media: N/A (Valores otimos nao disponiveis no arquivo)\n";
    }
    std::cout << "- Tempo Medio de Execucao: " << std::fixed << std::setprecision( 2 ) << ( sumTime / instances.size() ) << " ms\n";
}

int main(){
    std::cout << "======================================================\n";
    std::cout << "   RESOLVEDOR DE MOCHILA MULTIDIMENSIONAL (MKP)\n";
    std::cout << "             Simulated Annealing C++\n";
    std::cout << "======================================================\n\n";

    std::string filename;
    std::cout << "Digite o caminho do arquivo de testes (ex: data/mknap1.txt): ";
    std::cin >> filename;

    std::vector<Instance> instances = readInstances( filename );
    if( instances.empty() ){
        std::cout << "Nenhuma instancia carregada. Verifique se o caminho esta correto.\n";
        return 1;
    }

    std::cout << "\nForam carregadas " << instances.size() << " instancias.\n";
    for( const auto& inst : instances ){
        std::cout << "  Instancia " << inst.id << ": n=" << inst.n << ", m=" << inst.m << ", Opt=" << inst.optimalValue << "\n";
    }

    double t0 = 1000.0;
    double alpha = 0.98;
    double tMin = 0.01;
    int L = 1000;

    std::cout << "\n--- Parametros do Simulated Annealing ---\n";
    std::cout << "Deseja usar os parametros padrao (T0=1000, alpha=0.98, T_min=0.01, L=1000)?\n";
    std::cout << "1. Sim (Recomendado)\n2. Nao (Personalizar)\n";
    int paramChoice = getMenuChoice( 1, 2 );

    if( paramChoice == 2 ){
        std::cout << "Digite a Temperatura Inicial (T0): ";
        std::cin >> t0;
        std::cout << "Digite a Taxa de Resfriamento (alpha [0.80 - 0.999]): ";
        std::cin >> alpha;
        std::cout << "Digite a Temperatura Minima (T_min): ";
        std::cin >> tMin;
        std::cout << "Digite a quantidade de iteracoes por patamar (L): ";
        std::cin >> L;
    }

    std::cout << "\n--- Modo de Execucao ---\n";
    std::cout << "1. Executar apenas UMA instancia\n";
    std::cout << "2. Executar TODAS as instancias em lote (Batch)\n";
    int execMode = getMenuChoice( 1, 2 );

    if( execMode == 1 ){
        std::cout << "Digite o ID da instancia que deseja executar (1-" << instances.size() << "): ";
        int instId = getMenuChoice( 1, instances.size() );
        runSingleInstance( instances[instId - 1], t0, alpha, tMin, L );
    } else {
        runBatchInstances( instances, t0, alpha, tMin, L );
    }

    std::cout << "\nExecucao encerrada com sucesso.\n";
    return 0;
}
