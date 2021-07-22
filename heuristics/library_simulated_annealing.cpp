#include "../class_Random.cpp"

#include "auto_util_header.hpp"

namespace heur_binary {

    using solution_t = std::vector<unsigned char>;

    using evalval_t = int64_t;

    class Simulated_Annealing {

        size_t n;
        solution_t X_cur;
        solution_t X_best;
        evalval_t evalval_cur;
        evalval_t evalval_best;
        std::shared_ptr<Random_Dynamic_Int> rdi;
        std::shared_ptr<Random_Real> rr;
        std::shared_ptr<Selector> selector;

        std::function<evalval_t(
            const solution_t &
        )> objective_function;

        std::vector<std::function<evalval_t(
            const solution_t &
        )>> relational_functions;

        std::function<solution_t(
            const size_t n,
            const std::shared_ptr<Random_Dynamic_Int> rdi
        )> initializing_function;

        std::function<std::vector<size_t>(
            const solution_t &X,
            const std::shared_ptr<Random_Dynamic_Int>,
            std::shared_ptr<Selector>
        )> neighboring_function;

        std::function<double(
            size_t,
            size_t
        )> temperature_function;

        double calc_probability(
            const evalval_t prev_val,
            const evalval_t next_val,
            const double temperature
        ) {
            // return std::exp((prev_val - next_val) / temperature);
            double x = (prev_val - next_val) / temperature;
            return (((1.0 / 6) * x + (1.0 / 2)) * x + 1.0) * x + 1;
        }

        evalval_t calc_evaldiff_with_solution_update (
            solution_t &X,
            const std::vector<size_t> &flip_ids,
            const std::vector<std::function<evalval_t(const solution_t &)>> &relfunc
        ) {
            evalval_t ret = 0;
            for (const size_t flip_id : flip_ids) {
                ret -= relfunc[flip_id](X);
                X[flip_id] ^= 1;
                ret += relfunc[flip_id](X);
            }
            return ret;
        }

        evalval_t calc_evalval (
            const solution_t &X,
            const std::function<evalval_t(const solution_t &)> &objfunc
        ) {
            return objfunc(X);
        }

        void update(
            solution_t &X,
            const std::vector<size_t> &flip_ids
        ) {
            for (const size_t flip_id : flip_ids) {
                X[flip_id] ^= 1;
            }
        }

        void revert(
            solution_t &X,
            const std::vector<size_t> &flip_ids
        ) {
            for (const size_t flip_id : flip_ids) {
                X[flip_id] ^= 1;
            }
        }

    public:

        Simulated_Annealing (
            const size_t n,
            const std::function<evalval_t(
                const solution_t &
            )> objective_function,
            const std::vector<std::function<evalval_t(
                const solution_t &
            )>> &relational_functions,
            std::function<solution_t(
                const size_t n,
                const std::shared_ptr<Random_Dynamic_Int> rdi
            )> initializing_function,
            std::function<std::vector<size_t>(
                const solution_t &X,
                const std::shared_ptr<Random_Dynamic_Int>,
                std::shared_ptr<Selector>
            )> neighboring_function,
            std::function<double(
                size_t,
                size_t
            )> temperature_function
        ) {
            this->n = n;
            this->objective_function = objective_function;
            this->relational_functions = relational_functions;
            this->initializing_function = initializing_function;
            this->neighboring_function = neighboring_function;
            this->temperature_function = temperature_function;
            this->rdi = std::make_shared<Random_Dynamic_Int>();
            this->rr = std::make_shared<Random_Real>(0, 1);
            this->selector = std::make_shared<Selector>(this->n);
        }

        solution_t get_solution(
            const size_t iter_max
        ) {
            X_cur = initializing_function(n, rdi);
            evalval_cur = calc_evalval(X_cur, objective_function);
            X_best = X_cur;
            evalval_best = evalval_cur;
            for (size_t iter_cur = 0; iter_cur < iter_max; ++iter_cur) {
                std::vector<size_t> flip_ids = neighboring_function(X_cur, rdi, selector);
                evalval_t evaldiff = calc_evaldiff_with_solution_update(X_cur, flip_ids, relational_functions);
                evalval_t evalval_next = evalval_cur + evaldiff;
                // std::cout << evalval_cur << " " << evalval_next << " " << temperature_function(iter_cur, iter_max) << " " << calc_probability(evalval_cur, evalval_next, temperature_function(iter_cur, iter_max)) << std::endl;
                if (rr->get() < calc_probability(evalval_cur, evalval_next, temperature_function(iter_cur, iter_max))) {
                    evalval_cur = evalval_next;
                    if (evalval_best > evalval_cur) {
                        X_best = X_cur;
                        evalval_best = evalval_cur;
                    }
                }
                else {
                    revert(X_cur, flip_ids);
                }
            }
            return X_best;
        }

    };

    namespace helper {

        std::function<evalval_t(const solution_t &)> convert_polynomial_to_objfunc (
            const size_t n,
            const std::vector<std::pair<std::vector<size_t>, evalval_t>> &P
        ) {

            auto zero = [](const solution_t &X) {
                return evalval_t(0);
            };

            std::function<evalval_t(const solution_t &)> objfunc = zero;

            for (const auto &term : P) {
                objfunc = [term, prev = std::move(objfunc)](const solution_t &X) {
                    auto buf = 1;
                    for (const auto k : term.first) {
                        buf &= X[k];
                    }
                    evalval_t coeff = term.second;
                    return prev(X) + buf * coeff;
                };
            }

            return objfunc;

        }

        std::vector<std::function<evalval_t(const solution_t &)>> convert_polynomial_to_relfuncs (
            const size_t n,
            const std::vector<std::pair<std::vector<size_t>, evalval_t>> &P
        ) {

            auto zero = [](const solution_t &X) {
                return evalval_t(0);
            };

            std::vector<std::function<evalval_t(const solution_t &)>> relfuncs(n, zero);

            for (const auto &term : P) {
                for (const auto i : term.first) {
                    relfuncs[i] = [i, term, prev = std::move(relfuncs[i])](const solution_t &X) {
                        auto buf = 1;
                        for (const auto k : term.first) {
                            buf &= X[k];
                        }
                        evalval_t coeff = term.second;
                        return prev(X) + buf * coeff;
                    };
                }
            }

            for (size_t i = 0; i < n; ++i) {
                relfuncs[i] = [i, prev = relfuncs[i]](const solution_t &X) {
                    if (X[i]) {
                        return prev(X);
                    }
                    else {
                        return evalval_t(0);
                    }
                };
            }

            return relfuncs;

        }

        std::function<double(size_t, size_t)> create_exponential_temperature_function(
            const double scale,
            const double alpha = 0.5
        ) {
            std::array<double, 4> log_alpha;
            for (size_t i = 0; i < log_alpha.size(); ++i) {
                log_alpha[i] = std::pow(std::log(alpha), i);
            }
            auto tfunc = [scale, log_alpha](size_t iter_cur, size_t iter_max) {
                // return std::pow(alpha, iter_cur / iter_max)
                auto x = double(iter_cur) / iter_max;
                return scale * std::max(log_alpha[1], ((log_alpha[3] / 6 * x + log_alpha[2] / 2) * x + log_alpha[1]) * x + log_alpha[0]);
            };
            return tfunc;
        }

    }

    // customizable
    solution_t choose_initial_solution(
        const size_t n,
        const std::shared_ptr<Random_Dynamic_Int> rdi
    ) {
        solution_t ret(n, 0);
        for (size_t i = 0; i < n; ++i) {
            ret[i] = rdi->get(0, 2);
        }
        return ret;
    }

    // customizable
    std::vector<size_t> choose_neighbor (
        const solution_t &X,
        const std::shared_ptr<Random_Dynamic_Int> rdi,
        const std::shared_ptr<Selector> selector
    ) {
        const size_t n = X.size();
        const size_t n_flip = rdi->get(1, std::min(n, size_t(std::sqrt(n))));
        return selector->get(n_flip);
    }

};


/*

int main() {

    size_t n = 400;

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

    if (n <= 20) {

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


*/
