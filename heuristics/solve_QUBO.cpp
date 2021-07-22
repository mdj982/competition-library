#include <bits/stdc++.h>
#include "library_tabu_search.cpp"
#include "library_simulated_annealing.cpp"
#include "../include/class_Timestamp.hpp"

int main() {

    size_t n = 500;

    std::vector<int64_t> M(n * n);

    auto rdi = std::make_unique<Random_Dynamic_Int>();

    std::vector<std::pair<std::vector<std::size_t>, heur_binary::evalval_t>> P;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            M[i * n + j] = rdi->get(-100, 100);
            if (i == j) {
                P.push_back({{i}, M[i * n + j]});
            }
            else {
                P.push_back({{i, j}, M[i * n + j]});
            }
        }
    }

    auto objfunc = heur_binary::helper::convert_polynomial_to_objfunc(n, P);
    auto relfuncs = heur_binary::helper::convert_polynomial_to_relfuncs(n, P);

    if (n <= 22) {

        std::cout << "method: BRUTE FORCE" << std::endl;

        heur_binary::solution_t X(n);


        double evalmin = INFINITY;
        double evalmax = -INFINITY;

        for (int code = 0; code < (1 << n); ++code) {
            for (size_t i = 0; i < n; ++i) {
                X[i] = (code >> i) & 1;
            }
            double evalbuf = objfunc(X);
            if (evalmin > evalbuf) {
                evalmin = evalbuf;
            }
            if (evalmax < evalbuf) {
                evalmax = evalbuf;
            }
        }

        std::cout << "min = " << evalmin << std::endl;
        std::cout << "max = " << evalmax << std::endl;
    }

    {
        std::cout << "method: TABU SEARCH" << std::endl;
        auto solver = std::make_unique<heur_qubo::Tabu_Search>(n, M);
        solver->simple_tabu_search(250000000 / n);
        auto X_ans = solver->get_best_solution();
        auto val_ans = solver->get_best_value();
        assert(val_ans == heur_qubo::calc_evalval(n, M, X_ans));
        std::cout << val_ans << std::endl;
    }
    {
        std::cout << "method: SIMULATED ANNEALING" << std::endl;
        auto solver = std::make_unique<heur_binary::Simulated_Annealing>(
            n,
            objfunc,
            std::move(relfuncs),
            heur_binary::choose_initial_solution,
            heur_binary::choose_neighbor,
            heur_binary::helper::create_exponential_temperature_function(50)
        );
        size_t N_ITERATION = size_t(1e8 * (2 / std::sqrt(n) / n));
        std::cout << "N_ITERATION = " << N_ITERATION << std::endl;
        auto Y = solver->get_solution(N_ITERATION);
        std::cout << objfunc(Y) << std::endl;
    }

}
