#include "../include/instance.hpp"
#include "../include/solution.hpp"
#include "../include/sa.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>

// Funcao auxiliar para ler a escolha do usuario
int get_menu_choice(int min_val, int max_val) {
    int choice;
    while (true) {
        std::cout << "Escolha uma opcao (" << min_val << "-" << max_val << "): ";
        if (std::cin >> choice && choice >= min_val && choice <= max_val) {
            return choice;
        }
        std::cin.clear();
        std::cin.ignore(32767, '\n');
        std::cout << "Opcao invalida. Tente novamente.\n";
    }
}

// Executa o SA em uma unica instancia e imprime o resultado
void run_single_instance(const Instance& inst, double t0, double alpha, double t_min, int L) {
    std::cout << "\n=============================================\n";
    std::cout << "Executando Instancia ID: " << inst.id << " (n=" << inst.n << ", m=" << inst.m << ")\n";
    std::cout << "=============================================\n";

    auto start_time = std::chrono::high_resolution_clock::now();

    SimulatedAnnealing sa(t0, alpha, t_min, L);
    Solution best_sol = sa.run(inst);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    best_sol.print(inst);

    std::cout << "\n---------------- CORRETUDE ----------------\n";
    if (best_sol.is_valid) {
        std::cout << "Solucao VIAVEL (Nenhuma capacidade de recurso foi estourada).\n";
    } else {
        std::cout << "AVISO: Solucao INVIABEL (Estouro de recursos!)\n";
    }

    if (inst.optimal_value > 0) {
        double gap = ((inst.optimal_value - best_sol.total_profit) / inst.optimal_value) * 100.0;
        double quality = (best_sol.total_profit / inst.optimal_value) * 100.0;
        std::cout << "Valor Otimo Teorico: " << inst.optimal_value << "\n";
        std::cout << "Qualidade da Solucao: " << std::fixed << std::setprecision(2) << quality << "%\n";
        std::cout << "GAP (Distancia do Otimo): " << gap << "%\n";
    } else {
        std::cout << "Valor Otimo Teorico: Desconhecido (0)\n";
    }

    std::cout << "Tempo de Execucao: " << std::fixed << std::setprecision(2) << duration.count() << " ms\n";
}

// Executa todas as instancias em lote e apresenta uma tabela final
void run_batch_instances(const std::vector<Instance>& instances, double t0, double alpha, double t_min, int L) {
    std::cout << "\n=============================================\n";
    std::cout << "Executando todas as " << instances.size() << " instancias em lote (Batch Mode)\n";
    std::cout << "=============================================\n";

    struct ResultRow {
        int id;
        int n;
        int m;
        double opt;
        double found;
        double quality;
        double duration_ms;
        bool valid;
    };

    std::vector<ResultRow> results;

    for (const auto& inst : instances) {
        std::cout << "Rodando instancia " << inst.id << " / " << instances.size() << "..." << std::flush;
        
        auto start = std::chrono::high_resolution_clock::now();
        SimulatedAnnealing sa(t0, alpha, t_min, L);
        Solution best_sol = sa.run(inst);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;

        double quality = (inst.optimal_value > 0) ? (best_sol.total_profit / inst.optimal_value) * 100.0 : 0.0;

        results.push_back({
            inst.id,
            inst.n,
            inst.m,
            inst.optimal_value,
            best_sol.total_profit,
            quality,
            duration.count(),
            best_sol.is_valid
        });
        std::cout << " Concluido (" << duration.count() << " ms)\n";
    }

    // Imprime a tabela em formato Markdown para colar no relatorio
    std::cout << "\n### TABELA DE RESULTADOS (Pronta para o Relatorio)\n\n";
    std::cout << "| Inst. |  n  |  m  | Opt. Teorico | Lucro Encontrado | Qualidade (%) | Tempo (ms) | Viavel |\n";
    std::cout << "|-------|-----|-----|--------------|------------------|---------------|------------|--------|\n";
    
    double sum_quality = 0.0;
    double sum_time = 0.0;
    int opt_count = 0;
    int instances_with_opt = 0;

    for (const auto& r : results) {
        std::cout << "| " << std::setw(5) << r.id
                  << " | " << std::setw(3) << r.n
                  << " | " << std::setw(3) << r.m;
        
        std::cout << std::fixed; // Garante que nao exibira em notacao cientifica (ex: 2e+04)
        
        if (r.opt > 0) {
            std::cout << " | " << std::setw(12) << std::setprecision(1) << r.opt
                      << " | " << std::setw(16) << std::setprecision(1) << r.found
                      << " | " << std::setw(12) << std::setprecision(2) << r.quality << "%";
            
            sum_quality += r.quality;
            instances_with_opt++;
            if (r.found >= r.opt) {
                opt_count++;
            }
        } else {
            std::cout << " | " << std::setw(12) << "N/A"
                      << " | " << std::setw(16) << std::setprecision(1) << r.found
                      << " | " << std::setw(13) << "N/A";
        }
        
        std::cout << " | " << std::setw(9) << std::setprecision(1) << r.duration_ms
                  << " | " << (r.valid ? "Sim" : "Nao") << " |\n";
        
        sum_time += r.duration_ms;
    }
    
    std::cout << "\n### Resumo Estatistico:\n";
    if (instances_with_opt > 0) {
        std::cout << "- Qualidade Media: " << std::fixed << std::setprecision(2) << (sum_quality / instances_with_opt) << "%\n";
        std::cout << "- Vezes que atingiu o Otimo: " << opt_count << " de " << instances_with_opt << "\n";
    } else {
        std::cout << "- Qualidade Media: N/A (Valores otimos nao disponiveis no arquivo)\n";
    }
    std::cout << "- Tempo Medio de Execucao: " << std::fixed << std::setprecision(2) << (sum_time / instances.size()) << " ms\n";
}

int main() {
    std::cout << "======================================================\n";
    std::cout << "   RESOLVEDOR DE MOCHILA MULTIDIMENSIONAL (MKP)\n";
    std::cout << "             Simulated Annealing C++\n";
    std::cout << "======================================================\n\n";

    std::string filename;
    std::cout << "Digite o caminho do arquivo de testes (ex: data/mknap1.txt): ";
    std::cin >> filename;

    std::vector<Instance> instances = read_instances(filename);
    if (instances.empty()) {
        std::cout << "Nenhuma instancia carregada. Verifique se o caminho esta correto.\n";
        return 1;
    }

    std::cout << "\nForam carregadas " << instances.size() << " instancias.\n";
    for (const auto& inst : instances) {
        std::cout << "  Instancia " << inst.id << ": n=" << inst.n << ", m=" << inst.m << ", Opt=" << inst.optimal_value << "\n";
    }

    // Configuracao de Parametros do Simulated Annealing
    double t0 = 1000.0;
    double alpha = 0.98;
    double t_min = 0.01;
    int L = 1000;

    std::cout << "\n--- Parametros do Simulated Annealing ---\n";
    std::cout << "Deseja usar os parametros padrao (T0=1000, alpha=0.98, T_min=0.01, L=1000)?\n";
    std::cout << "1. Sim (Recomendado)\n2. Nao (Personalizar)\n";
    int param_choice = get_menu_choice(1, 2);

    if (param_choice == 2) {
        std::cout << "Digite a Temperatura Inicial (T0): ";
        std::cin >> t0;
        std::cout << "Digite a Taxa de Resfriamento (alpha [0.80 - 0.999]): ";
        std::cin >> alpha;
        std::cout << "Digite a Temperatura Minima (T_min): ";
        std::cin >> t_min;
        std::cout << "Digite a quantidade de iteracoes por patamar (L): ";
        std::cin >> L;
    }

    // Menu de Execucao
    std::cout << "\n--- Modo de Execucao ---\n";
    std::cout << "1. Executar apenas UMA instancia\n";
    std::cout << "2. Executar TODAS as instancias em lote (Batch)\n";
    int exec_mode = get_menu_choice(1, 2);

    if (exec_mode == 1) {
        std::cout << "Digite o ID da instancia que deseja executar (1-" << instances.size() << "): ";
        int inst_id = get_menu_choice(1, instances.size());
        run_single_instance(instances[inst_id - 1], t0, alpha, t_min, L);
    } else {
        run_batch_instances(instances, t0, alpha, t_min, L);
    }

    std::cout << "\nExecucao encerrada com sucesso.\n";
    return 0;
}
