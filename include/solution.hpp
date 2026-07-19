#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "instance.hpp"
#include <vector>

struct Solution {
    std::vector<bool> x;                  // Vetor binario indicando se o item j esta na mochila (tamanho n)
    double total_profit;                  // Lucro total da solucao
    std::vector<double> resource_usage;   // Consumo atual de cada recurso (tamanho m)
    bool is_valid;                        // Indica se a solucao respeita todos os limites

    // Construtores
    Solution();
    Solution(int n);

    // Avalia o lucro e o consumo de recursos da solucao
    void evaluate(const Instance& inst);

    // Heuristica de reparacao para consertar solucoes invalidas
    // Remove itens de menor utilidade ate a solucao se tornar viavel
    void repair(const Instance& inst);

    // Altera a solucao atual invertendo o estado de um item aleatorio
    // e entao reavalia e repara para garantir viabilidade
    void generate_neighbor(const Instance& inst);

    // Exibe informacoes detalhadas da solucao
    void print(const Instance& inst) const;
};

#endif // SOLUTION_HPP
