#include "../include/sa.hpp"
#include <cmath>
#include <random>
#include <iostream>

// Reuso do gerador de numeros aleatorios
static std::mt19937 rng(std::random_device{}());

SimulatedAnnealing::SimulatedAnnealing(double t0, double alpha, double t_min, int L)
    : initial_temp(t0), cooling_rate(alpha), final_temp(t_min), iterations_per_temp(L) {}

Solution SimulatedAnnealing::run(const Instance& inst) {
    // 1. Inicializa solucao vazia (todos os itens desmarcados, 100% viavel)
    Solution S(inst.n);
    S.repair(inst); // Garante que esta devidamente inicializada e avaliada
    
    Solution S_best = S;
    double T = initial_temp;
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Loop principal de temperatura
    while (T > final_temp) {
        for (int step = 0; step < iterations_per_temp; ++step) {
            // Gera solucao vizinha
            Solution S_new = S;
            S_new.generate_neighbor(inst);

            // Calcula a diferenca de energia (lucro)
            // Queremos MAXIMIZAR o lucro, entao delta_E positivo indica melhora
            double delta_E = S_new.total_profit - S.total_profit;

            if (delta_E > 0) {
                // Melhora sempre e aceita
                S = S_new;
                if (S.total_profit > S_best.total_profit) {
                    S_best = S;
                }
            } else {
                // Pioras sao aceitas com uma probabilidade termodinamica
                double p = std::exp(delta_E / T);
                if (dist(rng) < p) {
                    S = S_new;
                }
            }
        }
        
        // Resfriamento geometrico
        T *= cooling_rate;
    }

    return S_best;
}
