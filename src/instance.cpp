#include "../include/instance.hpp"
#include <iostream>
#include <fstream>
#include <cctype>

void Instance::print_summary() const {
    std::cout << "Instancia ID: " << id << "\n"
              << "  Itens (n): " << n << "\n"
              << "  Recursos (m): " << m << "\n"
              << "  Valor Otimo Teorico: " << optimal_value << "\n";
    std::cout << "  Capacidades de Recursos: [";
    for (int i = 0; i < m; ++i) {
        std::cout << capacities[i] << (i == m - 1 ? "" : ", ");
    }
    std::cout << "]\n";
}

std::vector<Instance> read_instances(const std::string& filepath) {
    std::vector<Instance> instances;
    std::ifstream infile(filepath);
    
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filepath << "\n";
        return instances;
    }

    // Le a primeira linha para determinar o formato
    std::string first_line;
    if (!std::getline(infile, first_line)) {
        std::cerr << "Erro: Arquivo vazio.\n";
        return instances;
    }

    // Verifica se a primeira linha contem apenas digitos e espacos (formato mknap1.txt)
    bool is_mknap1 = true;
    for (char c : first_line) {
        if (!std::isspace(c) && !std::isdigit(c)) {
            is_mknap1 = false;
            break;
        }
    }

    // Volta o ponteiro de leitura para o inicio do arquivo
    infile.clear();
    infile.seekg(0, std::ios::beg);

    if (is_mknap1) {
        // ==========================================
        // Formato mknap1.txt (OR-Library Padrao)
        // ==========================================
        int K = 0;
        if (!(infile >> K)) {
            std::cerr << "Erro ao ler a quantidade de instancias (K).\n";
            return instances;
        }

        for (int k = 0; k < K; ++k) {
            Instance inst;
            inst.id = k + 1;
            
            if (!(infile >> inst.n >> inst.m >> inst.optimal_value)) {
                break; // Fim do arquivo ou erro de leitura
            }

            // 1. Lucros (n valores)
            inst.profits.resize(inst.n);
            for (int j = 0; j < inst.n; ++j) {
                if (!(infile >> inst.profits[j])) {
                    std::cerr << "Erro ao ler lucros da instancia " << inst.id << "\n";
                    return instances;
                }
            }

            // 2. Matriz de restricoes (m x n)
            inst.constraints.assign(inst.m, std::vector<double>(inst.n, 0.0));
            for (int i = 0; i < inst.m; ++i) {
                for (int j = 0; j < inst.n; ++j) {
                    if (!(infile >> inst.constraints[i][j])) {
                        std::cerr << "Erro ao ler restricoes da instancia " << inst.id << "\n";
                        return instances;
                    }
                }
            }

            // 3. Capacidades (m valores)
            inst.capacities.resize(inst.m);
            for (int i = 0; i < inst.m; ++i) {
                if (!(infile >> inst.capacities[i])) {
                    std::cerr << "Erro ao ler capacidades da instancia " << inst.id << "\n";
                    return instances;
                }
            }

            instances.push_back(inst);
        }
    } else {
        // ==========================================
        // Formato mknap2.txt (Weingartner/Ness com cabecalhos)
        // ==========================================
        std::string line;
        int inst_count = 0;

        while (std::getline(infile, line)) {
            // Identifica o inicio de uma nova instancia pelo rotulo "problem"
            if (line.find("problem") != std::string::npos) {
                // Pula a linha seguinte que contem a linha de delimitadores (ex: ++++++)
                std::getline(infile, line);

                Instance inst;
                inst.id = ++inst_count;

                // O formato do mknap2.txt usa: m (restricoes) seguido de n (variaveis)
                if (!(infile >> inst.m >> inst.n)) {
                    std::cerr << "Erro ao ler dimensoes (m n) da instancia " << inst.id << "\n";
                    break;
                }

                // 1. Ler lucros/valores dos n itens
                inst.profits.resize(inst.n);
                for (int j = 0; j < inst.n; ++j) {
                    if (!(infile >> inst.profits[j])) {
                        std::cerr << "Erro ao ler lucros da instancia " << inst.id << "\n";
                        return instances;
                    }
                }

                // 2. Ler capacidades dos m recursos
                inst.capacities.resize(inst.m);
                for (int i = 0; i < inst.m; ++i) {
                    if (!(infile >> inst.capacities[i])) {
                        std::cerr << "Erro ao ler capacidades da instancia " << inst.id << "\n";
                        return instances;
                    }
                }

                // 3. Ler matriz de coeficientes (m x n)
                inst.constraints.assign(inst.m, std::vector<double>(inst.n, 0.0));
                for (int i = 0; i < inst.m; ++i) {
                    for (int j = 0; j < inst.n; ++j) {
                        if (!(infile >> inst.constraints[i][j])) {
                            std::cerr << "Erro ao ler restricoes da instancia " << inst.id << "\n";
                            return instances;
                        }
                    }
                }

                // 4. Ler valor otimo teorico
                if (!(infile >> inst.optimal_value)) {
                    std::cerr << "Erro ao ler valor otimo da instancia " << inst.id << "\n";
                    return instances;
                }

                instances.push_back(inst);
            }
        }
    }

    infile.close();
    return instances;
}
