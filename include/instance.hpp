#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vector>
#include <string>

struct Instance {
    int id;                                       // Identificador da instância (1, 2, ...)
    int n;                                        // Número de itens (variáveis)
    int m;                                        // Número de restrições (recursos)
    double optimal_value;                         // Valor ótimo conhecido (0 se desconhecido)
    std::vector<double> profits;                  // Vetor de lucro de cada item (tamanho n)
    std::vector<std::vector<double>> constraints; // Matriz de consumo de recursos (m x n)
    std::vector<double> capacities;               // Capacidade máxima de cada recurso (tamanho m)

    // Exibe informações básicas da instância para depuração
    void print_summary() const;
};

// Parser para ler todas as instâncias contidas em um arquivo no formato OR-Library
std::vector<Instance> read_instances(const std::string& filepath);

#endif // INSTANCE_HPP
