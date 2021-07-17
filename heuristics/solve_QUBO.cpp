#include <bits/stdc++.h>
#include "library_simulated_annealing.cpp"

int main() {

    size_t n = 22;

    std::vector<std::vector<double>> M(n, std::vector<double>(n));

    auto rdi = std::make_unique<Random_Dynamic_Int>();

    std::vector<std::pair<std::vector<std::size_t>, heur_binary::evalval_t>> P;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            M[i][j] = rdi->get(-100, 100);
            if (i == j) {
                P.push_back({{i}, M[i][j]});
            }
            else {
                P.push_back({{i, j}, M[i][j]});
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

    auto solver = std::make_unique<heur_binary::Simulated_Annealing>(
        n,
        objfunc,
        relfuncs,
        heur_binary::choose_initial_solution,
        heur_binary::choose_neighbor,
        heur_binary::helper::create_exponential_temperature_function(50)
    );

    auto Y = solver->get_solution(1000);
    std::cout << objfunc(Y) << std::endl;

}
