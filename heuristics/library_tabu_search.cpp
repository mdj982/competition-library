#include "../class_Random.cpp"

#include "auto_util_header.hpp"

namespace heur_qubo {

    using solution_t = std::vector<unsigned char>;

    using evalval_t = int64_t;

    // return upper triangular matrix M <- C with respect to 0 <- X
    // cost_matrix: upper triangular matrix
    std::vector<evalval_t> calc_diff_matrix(
        const size_t n,
        const std::vector<evalval_t> &cost_matrix,
        const solution_t &X
    ) {
        std::vector<evalval_t> M(n * n, evalval_t());
        const std::vector<evalval_t> &C = cost_matrix;
        for (size_t i = 0; i < n; ++i) {
            {
                // i == j
                const size_t dst_id = i * n + i;
                evalval_t buf = 0;
                for (size_t k = 0; k < i; ++k) {
                    const size_t src_id = k * n + i;
                    buf += C[src_id] * X[k];
                }
                {
                    const size_t src_id = dst_id;
                    buf += C[src_id];
                }
                for (size_t k = i + 1; k < n; ++k) {
                    const size_t src_id = i * n + k;
                    buf += C[src_id] * X[k];
                }
                M[dst_id] = X[i] ? -buf : buf;
            }
            for (size_t j = i + 1; j < n; ++j) {
                // i < j
                const size_t dst_id = i * n + j;
                const size_t src_id = dst_id;
                M[dst_id] = (X[i] == X[j]) ? C[src_id] : -C[src_id];
            }
        }
        return M;
    }

    // update upper triangular matrix C_next <- C with respect to 0 <- X(1 in flip_id, 0 otherwise)
    // cost_matrix: upper triangular matrix
    void update_diff_matrix_by_flip(
        const size_t n,
        std::vector<evalval_t> &cost_matrix,
        const size_t flip_id
    ) {
        std::vector<evalval_t> &C = cost_matrix;
        {
            const size_t i = flip_id;
            for (size_t k = 0; k < i; ++k) {
                const size_t dst_id = k * n + k;
                const size_t src_id = k * n + i;
                C[dst_id] += C[src_id];
                C[src_id] *= -1;
            }
            {
                const size_t src_id = i * n + i;
                C[src_id] *= -1;
            }
            for (size_t k = i + 1; k < n; ++k) {
                const size_t dst_id = k * n + k;
                const size_t src_id = i * n + k;
                C[dst_id] += C[src_id];
                C[src_id] *= -1;
            }
        }
    }

    evalval_t calc_evalval(
        const size_t n,
        const std::vector<evalval_t> &cost_matrix,
        const solution_t &X
    ) {
        evalval_t val = 0;
        const std::vector<evalval_t> &C = cost_matrix;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i; j < n; ++j) {
                const size_t src_id = i * n + j;
                val += C[src_id] * X[i] * X[j];
            }
        }
        return val;
    }

    class Tabu_Search {
    private:
        size_t n;
        std::vector<evalval_t> C;
        std::vector<evalval_t> dC;
        solution_t X_cur;
        solution_t X_best;
        evalval_t evalval_cur;
        evalval_t evalval_best;
        void flip(
            const size_t flip_id,
            size_t &scan_cur
        ) {
            X_cur[flip_id] ^= 1;
            evalval_cur += dC[flip_id * n + flip_id];
            update_diff_matrix_by_flip(n, dC, flip_id);
            scan_cur++;
        }
        void record_best(
            size_t &scan_cur
        ) {
            if (evalval_cur < evalval_best) {
                X_best = X_cur;
                evalval_best = evalval_cur;
            }
            scan_cur++;
        }
        void local_search(
            size_t &scan_cur
        ) {
            bool find_best = false;
            do {
                find_best = false;
                for (size_t i = 0; i < n; ++i) {
                    const size_t src_id = i * n + i;
                    if (dC[src_id] < 0) {
                        find_best = true;
                        flip(i, scan_cur);
                    }
                }
                scan_cur++;
            } while (find_best);
        }
    public:
        Tabu_Search(
            const size_t n,
            const std::vector<evalval_t> &cost_matrix
        ) {
            this->n = n;
            this->C = cost_matrix;
            this->dC = cost_matrix;
            X_cur = solution_t(n, 0);
            X_best = X_cur;
            evalval_cur = 0;
            evalval_best = evalval_cur;
        }
        void simple_tabu_search(
            const size_t scan_max
        ) {
            // parameters
            const size_t TABU_TENURE_VALUE = std::min(size_t(20), this->n / 4);
            // procedure
            std::vector<unsigned int> flag_tabu(n, 0);
            for (size_t scan_cur = 0; scan_cur < scan_max;) {
                evalval_t evaldiff_bestnbr = std::numeric_limits<evalval_t>::max();
                size_t id_best = 0;
                bool flag_best = false;
                for (size_t i = 0; i < n; ++i) {
                    if (flag_tabu[i] > 0) continue;
                    const size_t src_id = i * n + i;
                    if (evalval_cur + dC[src_id] < evalval_best) {
                        evaldiff_bestnbr = dC[src_id];
                        id_best = i;
                        flag_best = true;
                        break;
                    }
                    if (dC[src_id] < evaldiff_bestnbr) {
                        evaldiff_bestnbr = dC[src_id];
                        id_best = i;
                    }
                }
                scan_cur++;
                {
                    this->flip(id_best, scan_cur);
                }
                if (flag_best) {
                    this->local_search(scan_cur);
                    this->record_best(scan_cur);
                }
                for (size_t i = 0; i < n; ++i) {
                    if (flag_tabu[i] > 0) flag_tabu[i]--;
                    flag_tabu[id_best] = TABU_TENURE_VALUE;
                }
                scan_cur++;
            }
        }
        solution_t get_best_solution() {
            return this->X_best;
        }
        evalval_t get_best_value() {
            return this->evalval_best;
        }
    };

}
