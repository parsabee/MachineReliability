#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

void usage() {
    std::cerr << "usage: reliability [--iter] [--memo]\n";
}

/*
 * finding reliability recursively
 */
double reliability(int B, int index,
                   const std::vector<std::pair<int, double>> &vec,
                   std::vector<std::vector<double>> &memo,
                   std::vector<std::vector<int>> &copies) {

    /* base cases */
    if (B < 0) {
        return 0.0;
    } else if (B == 0 && index > 0) {
        return 0.0;
    } else if (B >= 0 && index == 0) {
        return 1.0;
    }

    /* recursive case */
    double max = 0.0;
    int possible_copies = B/vec[index].first;
    int rem_budget = B; /* remaining budget */
    int best_m = 0;
    for (int m = 1; m <= possible_copies; m++){
        rem_budget -= vec[index].first; /* subtract cost of machine i from remaining budget */
        if (memo[index-1][rem_budget] == -1.0){
            memo[index-1][rem_budget] = reliability(rem_budget, index-1, vec, memo, copies);
        }
        auto tmp = (1.0 - pow(1.0 - vec[index].second, m)) * memo[index-1][rem_budget];
        if (tmp > max) {
            max = tmp;
            best_m = m;
        }
    }
    memo[index][B] = max;
    copies[index][B] = best_m;
    return max;
}

/*
 * finding reliability iteratively
 */
double reliability_iter(int B, int index,
                   const std::vector<std::pair<int, double>> &vec,
                   std::vector<std::vector<double>> &memo,
                   std::vector<std::vector<int>> &copies) {

    /* iterate over every machine */
    for (int i = 1; i <= index; i++) {
        /* iterate over every possible budget */
        for (int budget = 1; budget <= B; budget++) {
            int possible_copies = budget/vec[i].first;
            double max = 0.0;
            int best_m = 0;
            /* base case */
            if (i == 1) {
                /* when we have only one machine, all budget can be spent on it*/
                max = 1.0 - pow(1.0 - vec[i].second, possible_copies);
                best_m = possible_copies;
            } else {
                int rem_budget = budget;
                for (int m = 1; m <= possible_copies; m++) {
                    rem_budget -= vec[i].first; /* remaining budget after deducting cost of m, i machines */
                    auto tmp = (1.0 - pow(1.0 - vec[i].second, m)) * memo[i-1][rem_budget];
                    if (tmp > max) {
                        max = tmp;
                        best_m = m;
                    }
                }
            }
            memo[i][budget] = max;
            copies[i][budget] = best_m;
        }
    }
    return memo[index][B];
}

void out_cost(int B, int index, 
              const std::vector<std::pair<int, double>> &vec,
              std::vector<std::vector<int>> &copies
              ) {
    /*
     * printing the number of cpoies per every machine
     */
    for (; index > 0; index--){
        std::cout << copies[index][B] << " copies of machine " << index 
                  << " of cost " << vec[index].first << "\n";
        B -= copies[index][B]*vec[index].first;
    }
}

int main(int argc, char *argv[]) {

    /* 
     * parsing options 
     */
    bool iterative = false, dynamic = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--iter") == 0) {
            iterative = true;
        } else if (strcmp(argv[i], "--memo") == 0) {
            dynamic = true;
        } else {
            std::cerr << "unkown option: " << argv[i] << "\n";
            usage();
            exit(1);
        }
    }
    if (!iterative && !dynamic) {
        iterative = true; dynamic = true; /* default is both iterative and memoized*/
    }

    int B, N; /* B: budget, N: number of machines */
    std::cin >> B >> N;
    std::vector<std::pair<int, double>> vec(N+1, {0, 0.0}); /* cost - reliability of every machine*/
    for(int i = 1; i <= N; i++){
        std::cin >> vec[i].first; /* cost */
        std::cin >> vec[i].second; /* reliability */
    }

    std::cout << "Budget: " << B << "\n";
    std::cout << "Number machines: " << N << "\n\n";

    if (iterative) {
        std::cout << "Iterated version:\n";
        std::vector<std::vector<double>> memo(N+1, std::vector<double>(B+1, 0.0));
        std::vector<std::vector<int>> copies(N+1, std::vector<int>(B+1, 0));
        std::cout << "Maximum reliability: " << reliability_iter(B, N, vec, memo, copies) << "\n";
        out_cost(B, N, vec, copies);
        std::cout << std::endl;
    }

    if (dynamic) {
        std::cout << "Memoized version:\n";
        std::vector<std::vector<double>> memo(N+1, std::vector<double>(B+1, -1.0));
        std::vector<std::vector<int>> copies(N+1, std::vector<int>(B+1, 0));
        std::cout << "Maximum reliability: " << reliability(B, N, vec, memo, copies) << "\n";
        out_cost(B, N, vec, copies);
        std::cout << std::endl;
    }
}