#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <set>
#include <deque>
#include  <random>
#include  <iterator>

template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> a) {
    for (const auto& v: a) {
        out  << v << " ";
    }
    return out; 
}

int is_covered_one_col(const std::vector<int>& a) {
    // если строка покрывается единственным столбцом, возвращает номер этого столбца
    // далее это столбец не трогать
    int ans = -1, i = 0;
    for (const auto& v: a) {
        if (v == 1) {
            if (ans != -1) return -1;
            ans = i;
        }
        i ++;
    }
    return ans;
}

std::pair<std::vector<std::vector<bool>>, 
    std::vector<int>> get_data(int &n, int& m) {
    std::ifstream in("tmp.data");
    in >> m >> n;
    std::vector<std::vector<bool>> data(m, std::vector<bool>(n, 0));
    std::vector<int> cost(n, 0);
    std::string s;
    std::getline(in, s);
    for (int i = 0; i < n; i++) {
        std::getline(in, s);
        std::stringstream in_stream(s);
        in_stream >> cost[i];
        int a;
        while (in_stream >> a) {
            data[a][i] = 1;
        }
    }
    return std::make_pair(data, cost);
}

int min(std::vector<int>& a) {
    int ans = a[0];
    for (const auto& v: a) {
        if (v < ans) ans = v;
    }
    return ans;
}

int max(std::vector<int>& a) {
    int ans = a[0];
    for (const auto& v: a) {
        if (v > ans) ans = v;
    }
    return ans;
}

std::set<int> get_n(int col, const std::vector<std::vector<bool>>& data) {
    std::set<int> neight;
    for (int i = 0; i < data.size(); i++) {
        if (!data[i][col]) continue;
        for (int j = 0; j < data[0].size(); j++) {
            if (j != col && data[i][j]) neight.insert(j);
        }
    }
    return neight;
}

void remove_col(int col, const std::vector<std::vector<bool>>& data,
    std::vector<int>& scores, std::vector<bool>& elig, std::vector<int>& covered,
    const std::vector<int>& weight, std::vector<bool>& sol, std::set<int>& uncover, bool to_add = false) {
    auto neigh = get_n(col, data);
    sol[col] = to_add;
    if (!to_add) {
        elig[col] = false;
        for (int i = 0; i < data.size(); i++) {
            if (data[i][col]) covered[i] -= 1;
            if (covered[i] == 0) uncover.insert(i);
        }
    }
    else {
        for (int i = 0; i < data.size(); i++) {
            if (data[i][col]) covered[i] += 1;
            if (covered[i] > 0) uncover.erase(i);
        }
    }
    scores[col] = -1 * scores[col];
    for (auto c: neigh) {
        elig[c] = true;
        scores[c] = 0;
        if (sol[c]) {
            for (int v = 0; v < data.size(); v++) {
                if (data[v][c] && covered[v] == 1) {
                    scores[c] -= weight[v];
                }
            }
        } else {
            for (int v = 0; v < data.size(); v++) {
                if (data[v][c] && covered[v] == 0) {
                    scores[c] += weight[v];
                }
            }
        }
    }
}

bool is_TABU(int col, const std::deque<int>& TABU) {
    for (const auto v: TABU) {
        if (v == col) return true;
    }
    return false;
}

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

int sum(std::vector<bool>& sol, std::vector<int>& cost) {
    int ans = 0;
    for (int i = 0; i < sol.size(); i++) ans += sol[i] * cost[i];
    return ans;
}

int main(int argc, char** argv) {
    int n = 0, m = 0;
    // m - size of universal set
    // n - size of F
    auto dat = get_data(n, m); // mxn
    auto data = dat.first;
    auto cost = dat.second;
    //std::cout << data << "\n";
    std::vector<bool> elig(n, true);
    std::vector<int> time(n, 1);
    std::vector<int> weight(m, 1);
    std::vector<int> scores(n, 0);
    /*int col1 = 5;
    std::set<int> neight;
    for (int i = 0; i < m; i++) {
        if (!data[i][col1]) continue;
        for (int j = 0; j < n; j++) {
            if (j != col1 && data[i][j]) neight.insert(j);
        }
    }
    for (const auto&v: neight) std::cout << v << " ";
    std::cout << "\n"; */
    std::vector<bool> sol(n, false);
    // random solution
    /*{
        std::set<int> get;
        std::set<int> cov;
        for (int i = 0; i < n; i++) {
            get.insert(i);
        }
        while (cov.size() < m) {
            int r = *select_randomly(get.begin(), get.end());
            get.erase(r);
            for (int i = 0; i < m; i++) {
                if (data[i][r]) cov.insert(i);
            }
        }
        for (auto v: get) {
            sol[v] = true;
        }
    } */
    // random solution 
    // greedy solution
    {
        std::ifstream init("init_sol.data");
        std::string line;
        std::getline(init, line);
        std::getline(init, line);
        int i = 0, v;
        std::stringstream init_str(line);
        while (init_str >> v) {
            sol[i] = v;
            i++;
        }

    }
    // greedy solution
    std::vector<bool> best_sol(n, true);
    best_sol = sol;
    std::vector<int> covered(m, 0); 
    int ub = 0;
    for (int i = 0; i < n; i++) ub += sol[i] * cost[i];
    int tot_cost = ub;
    for (int j = 0; j < n; j++) {
        if (!sol[j]) continue;
        for (int i = 0; i < m; i++) {
            covered[i] += data[i][j];
        }
    }
    int iter = 1;
    for (int i = 0; i < n; i++) {
        if (!sol[i]) {
            for (int v = 0; v < m; v++) {
                if (data[v][i] && covered[v] == 0) {
                    scores[i] += weight[v];
                }
            }
        }
        else {
            for (int v = 0; v < m; v++) {
                if (data[v][i] && covered[v] == 1) {
                    scores[i] -= weight[v];
                }
            }
        }
    }
    int min_cost = 0; 
    std::deque<int> TABU;
    std::set<int> uncover;
    int MAX_TABU = 7;
    int max_iter = atoi(argv[1]);
    while (iter <= max_iter) {
        while (min(covered) >= 1) {
            ub = tot_cost;
            double best = 1;
            best_sol = sol;
            int i_best = 0;
            for (int i = 0; i < n; i++) {
                if (!sol[i]) continue;
                double con_cost = double(scores[i]) / double(cost[i]);
                if (con_cost > best || best > 0) {
                    best = con_cost;
                    i_best = i;
                }
            }
            remove_col(i_best, data, scores, elig, covered, weight, sol, uncover);
            tot_cost -= cost[i_best];
        }
        double best = 1;
        int i_best, best_time = max_iter + 1;
        for (int i = 0; i < n; i++) {
            if (!sol[i]) continue;
            if (is_TABU(i, TABU)) continue;
            double con_cost = double(scores[i]) / double(cost[i]);
            if (con_cost > best || best > 0) {
                    best = con_cost;
                    i_best = i;
                    best_time = time[i];
            }
            if (con_cost == best && time[i] < best_time) {
                    best = con_cost;
                    i_best = i;
                    best_time = time[i];
            }
        }
        remove_col(i_best, data, scores, elig, covered, weight, sol, uncover);
        tot_cost -= cost[i_best];
        time[i_best] = iter;
        TABU.resize(0);
        while (min(covered) == 0) {
            int r = *select_randomly(uncover.begin(), uncover.end());
            double best = -10000000000;
            int i_best, best_time = max_iter + 1;
            for (int i = 0; i < n; i++) {
                if (!data[r][i]) continue;
                if (!elig[i]) continue;
                double con_cost = double(scores[i]) / double(cost[i]);
                if (con_cost > best + min_cost) {
                    best = con_cost;
                    i_best = i;
                    best_time = time[i];
                }
                if (con_cost == best && time[i] < best_time) {
                    best = con_cost;
                    i_best = i;
                    best_time = time[i];
                }
            }
            if (tot_cost + cost[i_best] >= ub + min_cost) break;
            remove_col(i_best, data, scores, elig, covered, weight, sol, uncover, true);  
            tot_cost += cost[i_best];  
            for (int i = 0; i < m; i++) {
                if (covered[i] > 0) weight[i] + 1;
            }
            TABU.push_back(i_best);
            //if (TABU.size() >= MAX_TABU) TABU.pop_front();
            time[i_best] = iter;
        }
        iter += 1;
        /* if (iter % 1000 == 0) {
            std::cout << "iter: " << iter << ": " << sum(best_sol, cost) << "\n";
            std::cout << "------------------------------------------------\n"; 
        } */
    }
    std::cout << sum(best_sol, cost) << " 0\n" << best_sol << "\n";
}