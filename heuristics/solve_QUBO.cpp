#include <bits/stdc++.h>
#include "library_tabu_search.cpp"
#include "library_simulated_annealing.cpp"
#include "../include/class_Timestamp.hpp"

int main() {

    size_t n = 1000;

    std::vector<int64_t> M(n * n);

    auto rdi = std::make_unique<Random_Dynamic_Int>();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            M[i * n + j] = rdi->get(-100, 100);
        }
    }

    if (n <= 22) {

        std::cout << "method: BRUTE FORCE" << std::endl;

        std::vector<std::pair<std::vector<std::size_t>, heur_binary::evalval_t>> P;

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i; j < n; ++j) {
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
        auto ri = std::make_unique<Random_Int>(2);
        for (size_t T = 0; T < 5; ++T) {
            heur_qubo::solution_t X;
            if (T == 0) {
                X.resize(n, 0);
            }
            else if (T == 1) {
                X.resize(n, 1);
            }
            else {
                X.resize(n);
                for (size_t i = 0; i < n; ++i) {
                    X[i] = ri->get();
                }
            }
            auto solver = std::make_unique<heur_qubo::Tabu_Search>(n, M, X);
            solver->simple_tabu_search(50000000 / n);
            auto X_ans = solver->get_best_solution();
            auto val_ans = solver->get_best_value();
            assert(val_ans == heur_qubo::calc_evalval(n, M, X_ans));
            std::cout << val_ans << std::endl;
        }
    }
    {
        std::cout << "method: TABU SEARCH0" << std::endl;
        auto ri = std::make_unique<Random_Int>(2);
        for (size_t T = 0; T < 1; ++T) {
            heur_qubo::solution_t X;
            if (T == 0) {
                X.resize(n, 0);
            }
            else if (T == 1) {
                X.resize(n, 1);
            }
            else {
                X.resize(n);
                for (size_t i = 0; i < n; ++i) {
                    X[i] = ri->get();
                }
            }
            auto solver = std::make_unique<heur_qubo::Tabu_Search>(n, M, X);
            solver->simple_tabu_search(250000000 / n);
            auto X_ans = solver->get_best_solution();
            auto val_ans = solver->get_best_value();
            assert(val_ans == heur_qubo::calc_evalval(n, M, X_ans));
            std::cout << val_ans << std::endl;
        }
    }
    {
        std::cout << "method: MST2" << std::endl;
        auto ri = std::make_unique<Random_Int>(2);
        for (size_t T = 0; T < 1; ++T) {
            heur_qubo::solution_t X;
            if (T == 0) {
                X.resize(n, 0);
            }
            else if (T == 1) {
                X.resize(n, 1);
            }
            else {
                X.resize(n);
                for (size_t i = 0; i < n; ++i) {
                    X[i] = ri->get();
                }
            }
            auto solver = std::make_unique<heur_qubo::Tabu_Search>(n, M, X);
            solver->tabu_search_based_sterategy(10000000 / n, 1000000 / n, 100000000 / n);
            auto X_ans = solver->get_best_solution();
            auto val_ans = solver->get_best_value();
            assert(val_ans == heur_qubo::calc_evalval(n, M, X_ans));
            std::cout << val_ans << std::endl;
        }
    }

}
