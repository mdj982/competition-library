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
            const size_t max_iter
        ) {
            X_cur = initializing_function(n, rdi);
            evalval_cur = calc_evalval(X_cur, objective_function);
            X_best = X_cur;
            evalval_best = evalval_cur;
            for (size_t cur_iter = 0; cur_iter < max_iter; ++cur_iter) {
                std::vector<size_t> flip_ids = neighboring_function(X_cur, rdi, selector);
                evalval_t evaldiff = calc_evaldiff_with_solution_update(X_cur, flip_ids, relational_functions);
                evalval_t evalval_next = evalval_cur + evaldiff;
                // std::cout << evalval_cur << " " << evalval_next << " " << temperature_function(cur_iter, max_iter) << " " << calc_probability(evalval_cur, evalval_next, temperature_function(cur_iter, max_iter)) << std::endl;
                if (rr->get() < calc_probability(evalval_cur, evalval_next, temperature_function(cur_iter, max_iter))) {
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
                objfunc = [term, prev = objfunc](const solution_t &X) {
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
                    relfuncs[i] = [i, term, prev = relfuncs[i]](const solution_t &X) {
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
            auto tfunc = [scale, log_alpha](size_t cur_iter, size_t max_iter) {
                // return std::pow(alpha, cur_iter / max_iter)
                auto x = double(cur_iter) / max_iter;
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
