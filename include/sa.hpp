#ifndef SA_HPP
#define SA_HPP

#include "instance.hpp"
#include "solution.hpp"

class SimulatedAnnealing {
private:
    double initial_temp;          // Temperatura inicial (T0)
    double cooling_rate;          // Taxa de resfriamento (alpha, ex: 0.95 a 0.99)
    double final_temp;            // Temperatura minima (critério de parada)
    int iterations_per_temp;      // Numero de iteracoes por temperatura (L)

public:
    // Construtor com parametros calibráveis
    SimulatedAnnealing(double t0, double alpha, double t_min, int L);

    // Executa o algoritmo para uma determinada instancia e retorna a melhor solucao
    Solution run(const Instance& inst);
};

#endif // SA_HPP
