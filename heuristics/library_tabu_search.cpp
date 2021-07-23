#include "../class_Random.cpp"

#include "auto_util_header.hpp"

#include <unistd.h>

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
        std::shared_ptr<Random_Dynamic_Real> rdr;

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

        void select_variables(
            std::vector<size_t> &ids_selected,
            size_t &scan_cur
        ) {
            // parameters
            const double lambda = 5000;
            // procedure
            std::vector<double> d_le(n);
            std::vector<double> d_ge(n);
            for (size_t i = 0; i < n; ++i) {
                const size_t src_id = i * n + i;
                d_le[i] = d_ge[i] = static_cast<double>(dC[src_id]);
            }
            scan_cur++;
            double skip_le = std::numeric_limits<double>::lowest();
            double skip_ge = std::numeric_limits<double>::max();
            std::vector<double> e(n);
            std::vector<double> acc_e(n + 1, 0);
            for (size_t &id_selected : ids_selected) {
                {
                    double d_max = *(std::max_element(d_le.begin(), d_le.end()));
                    double d_min = *(std::min_element(d_ge.begin(), d_ge.end()));
                    // calculate e
                    if (d_min == d_max) {
                        for (size_t i = 0; i < n; ++i) {
                            e[i] = 1;
                        }
                        scan_cur++;
                    }
                    else {
                        for (size_t i = 0; i < n; ++i) {
                            if (d_le[i] != skip_le) {
                                if (d_le[i] >= 0 && d_max > 0) {
                                    e[i] = 1 - d_le[i] / d_max;
                                }
                                else if (d_le[i] < 0) {
                                    e[i] = 1 + (d_le[i] / d_min) * lambda;
                                }
                                else {
                                    e[i] = 0;
                                }
                            }
                            else {
                                e[i] = 0;
                            }
                        }
                        scan_cur++;
                    }
                    // calculate acc_e
                    for (size_t i = 0; i < n; ++i) {
                        acc_e[i + 1] = acc_e[i] + e[i];
                    }
                    scan_cur++;
                }
                {
                    // choose id
                    std::vector<double>::iterator itr;
                    do {
                        double p = rdr->get(0.0, acc_e[n]);
                        itr = std::upper_bound(acc_e.begin(), acc_e.end(), p);
                    } while (itr == acc_e.begin() || itr == acc_e.end());
                    --itr;
                    id_selected = itr - acc_e.begin();
                }
                {
                    // update d
                    d_le[id_selected] = skip_le;
                    d_ge[id_selected] = skip_ge;
                    for (size_t i = 0; i < n; ++i) {
                        if (d_le[i] != skip_le) {
                            const size_t src_id = i < id_selected ? i * n + id_selected : id_selected * n + i;
                            d_le[i] = d_ge[i] = d_le[i] + dC[src_id];
                        }
                    }
                    scan_cur++;
                }
            }
            return;
        }

        void steepest_ascent(
            std::vector<size_t> &ids_selected,
            size_t &scan_cur
        ) {
            std::vector<evalval_t> h1(n);
            for (size_t i = 0; i < n; ++i) {
                const size_t src_id = i * n + i;
                h1[i] = dC[src_id];
            }
            scan_cur++;
            std::vector<evalval_t> h2(n, evalval_t());
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = i + 1; j < n; ++j) {
                    const size_t src_id = i * n + j;
                    h2[i] += dC[src_id];
                }
            }
            scan_cur += (n + 1) / 2;
            for (size_t _ = 0; _ < ids_selected.size(); ++_) {
                evalval_t v1 = std::numeric_limits<evalval_t>::max();
                evalval_t v2 = std::numeric_limits<evalval_t>::max();
                size_t id_best = SIZE_MAX;
                bool flag_flip = false;
                for (const size_t i : ids_selected) {
                    if (i != SIZE_MAX) {
                        evalval_t q1 = h1[i] * 2 + h2[i];
                        evalval_t q2 = h1[i];
                        bool flag_flip_buf;
                        if (q1 < 0 || (q1 == 0 && q2 <= 0)) {
                            flag_flip_buf = true;
                        }
                        else {
                            q1 *= -1;
                            q2 *= -1;
                            flag_flip_buf = false;
                        }
                        if (q1 < v1 || (q1 == v1 && q2 < v2)) {
                            v1 = q1;
                            v2 = q2;
                            id_best = i;
                            flag_flip = flag_flip_buf;
                        }
                    }
                }
                scan_cur++;
                for (size_t &i : ids_selected) {
                    if (i != SIZE_MAX) {
                        if (i != id_best) {
                            const size_t src_id = i < id_best ? i * n + id_best : id_best * n + i;
                            h2[i] -= dC[src_id];
                        }
                        else {
                            i = SIZE_MAX;
                        }
                    }
                }
                scan_cur++;
                if (flag_flip) {
                    for (const size_t &i : ids_selected) {
                        if (i != SIZE_MAX) {
                            const size_t src_id = i < id_best ? i * n + id_best : id_best * n + i;
                            h1[i] += dC[src_id];
                        }
                    }
                    scan_cur++;
                    this->flip(id_best, scan_cur);
                }
            }
        }

    public:

        Tabu_Search(
            const size_t n,
            const std::vector<evalval_t> &cost_matrix,
            const solution_t &X_init = {}
        ) {
            this->n = n;
            this->C = cost_matrix;
            if (X_init.size() != n) {
                this->X_cur = solution_t(n, 0);
            }
            else {
                this->X_cur = X_init;
            }
            {
                this->dC = cost_matrix;
                for (size_t i = 0; i < n; ++i) {
                    if (this->X_cur[i]) {
                        update_diff_matrix_by_flip(n, this->dC, i);
                    }
                }
            }
            this->evalval_cur = calc_evalval(n, this->C, this->X_cur);
            this->X_best = X_cur;
            this->evalval_best = evalval_cur;
            this->rdr = std::make_shared<Random_Dynamic_Real>();
        }

        void simple_tabu_search(
            const size_t scan_max
        ) {
            // parameters
            const size_t TABU_TENURE_VALUE = std::min(size_t(20), n / 4);
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

        void tabu_search_based_sterategy(
            const size_t scan_max1,
            const size_t scan_max2,
            const size_t scan_maxitr
        ) {
            // parameters
            const size_t CANDIDATE_SIZE = n * 4 / 10;
            // procedure
            this->simple_tabu_search(scan_max1);
            std::vector<size_t> ids_selected(CANDIDATE_SIZE, SIZE_MAX);
            std::cout << evalval_cur << "\t" << evalval_best << std::endl;
            for (size_t scan_cur = 0; scan_cur < scan_maxitr;) {
                this->select_variables(ids_selected, scan_cur);
                this->steepest_ascent(ids_selected, scan_cur);
                std::cout << "after ascent: \t" << evalval_cur << "\t" << evalval_best << std::endl;
                if (evalval_cur < evalval_best) {
                    this->record_best(scan_cur);
                }
                this->simple_tabu_search(scan_max2);
                scan_cur += scan_max2;
                std::cout << "after tabu: \t" << evalval_cur << "\t" << evalval_best << std::endl;
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
