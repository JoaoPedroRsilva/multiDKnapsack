#include "../include/solution.hpp"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

// Gerador de numeros aleatorios estatico
static std::mt19937 rng(std::random_device{}());

Solution::Solution() : total_profit(0.0), is_valid(true) {}

Solution::Solution(int n) : x(n, false), total_profit(0.0), resource_usage(0, 0.0), is_valid(true) {}

void Solution::evaluate(const Instance& inst) {
    total_profit = 0.0;
    resource_usage.assign(inst.m, 0.0);
    is_valid = true;

    for (int j = 0; j < inst.n; ++j) {
        if (x[j]) {
            total_profit += inst.profits[j];
            for (int i = 0; i < inst.m; ++i) {
                resource_usage[i] += inst.constraints[i][j];
            }
        }
    }

    // Verifica se violou alguma capacidade de recurso
    for (int i = 0; i < inst.m; ++i) {
        if (resource_usage[i] > inst.capacities[i]) {
            is_valid = false;
            break;
        }
    }
}

void Solution::repair(const Instance& inst) {
    evaluate(inst);
    if (is_valid) return;

    // Coleta todos os itens atualmente selecionados (x[j] == true)
    std::vector<int> selected_items;
    for (int j = 0; j < inst.n; ++j) {
        if (x[j]) {
            selected_items.push_back(j);
        }
    }

    // Calcula a relacao utilidade/consumo relativo para cada item selecionado.
    // Quanto menor o valor de ratio, pior e o item em termos de custo-beneficio multidimensional.
    std::vector<double> ratios(inst.n, 0.0);
    for (int j : selected_items) {
        double resource_sum = 0.0;
        for (int i = 0; i < inst.m; ++i) {
            if (inst.capacities[i] > 0) {
                resource_sum += inst.constraints[i][j] / inst.capacities[i];
            } else {
                resource_sum += inst.constraints[i][j];
            }
        }
        ratios[j] = inst.profits[j] / (resource_sum + 1e-9);
    }

    // Ordena os itens selecionados pelo ratio em ordem crescente (piores primeiro)
    std::sort(selected_items.begin(), selected_items.end(), [&](int a, int b) {
        return ratios[a] < ratios[b];
    });

    // Remove os piores itens ate que a solucao se torne valida
    for (int j : selected_items) {
        x[j] = false;
        total_profit -= inst.profits[j];
        
        bool current_valid = true;
        for (int i = 0; i < inst.m; ++i) {
            resource_usage[i] -= inst.constraints[i][j];
            if (resource_usage[i] > inst.capacities[i]) {
                current_valid = false;
            }
        }

        if (current_valid) {
            is_valid = true;
            break;
        }
    }
}

void Solution::generate_neighbor(const Instance& inst) {
    if (inst.n <= 0) return;

    // Inverte um bit aleatorio (0 -> 1 ou 1 -> 0)
    int j = rng() % inst.n;
    x[j] = !x[j];

    // Repara a solucao caso tenha ficado inviabilizada pela insercao do item
    repair(inst);
}

void Solution::print(const Instance& inst) const {
    std::cout << "Lucro Total: " << total_profit << "\n";
    std::cout << "Valida: " << (is_valid ? "Sim" : "Nao") << "\n";
    std::cout << "Recursos Utilizados:\n";
    for (int i = 0; i < inst.m; ++i) {
        double pct = (inst.capacities[i] > 0) ? (resource_usage[i] / inst.capacities[i]) * 100.0 : 0.0;
        std::cout << "  Recurso " << i + 1 << ": " << resource_usage[i] 
                  << " / " << inst.capacities[i] << " (" << pct << "%)\n";
    }
    std::cout << "Itens Selecionados: [ ";
    for (int j = 0; j < inst.n; ++j) {
        if (x[j]) std::cout << j + 1 << " ";
    }
    std::cout << "]\n";
}
