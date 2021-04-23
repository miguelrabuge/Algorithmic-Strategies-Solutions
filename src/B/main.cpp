#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>

#define ASCENDING (std::size_t) 0
#define DESCENDING (std::size_t) 1
#define MODULE 1000000007

std::array<std::array<std::array<int, 2>, 500>, 60000> dp_matrix;

int mod_abs(int a, int mod) {
	return ((a % mod) + mod) % mod;
}

int mod_add(int a, int b, int mod) {
	return (mod_abs(a, mod) + mod_abs(b, mod)) % mod;
}

int mod_sub(int a, int b, int mod) { 
	return mod_add(a, -b, mod);
}

void print_debug(std::size_t n,std::size_t depth, std::size_t mode) {
    for (std::size_t i = 0; i < depth; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            std::cout << dp_matrix[i][j][mode] << " ";
        }
        std::cout << "\n";
    }
}

int solve(std::size_t &h, std::size_t &n, std::size_t &depth) {
    std::size_t i, j, lowest;
    int sum = 0;
    dp_matrix[0][0][ASCENDING] = 1;

    // Ascending Step
    for (j = 1; j < n; ++j) {
        for (i = j; i < depth; ++i) {
            dp_matrix[i][j][ASCENDING] = mod_add(dp_matrix[i - 1][j][ASCENDING], dp_matrix[i - 1][j - 1][ASCENDING], MODULE);
            if (h <= i && dp_matrix[i - h][j - 1][ASCENDING]) 
                dp_matrix[i][j][ASCENDING] = mod_sub(dp_matrix[i][j][ASCENDING], dp_matrix[i - h][j - 1][ASCENDING], MODULE);
        }
    }

    // Descending Step
    for (j = 2; j < n; ++j) {
        lowest = std::min((h - 2) + (h - 1) * (j-2), depth - 2);
        dp_matrix[lowest][j][DESCENDING] = mod_add(dp_matrix[lowest][j][ASCENDING], dp_matrix[lowest + 1][j - 1][ASCENDING], MODULE);
        if (lowest > 0) {
            for (i = lowest - 1;; --i) {
                if (dp_matrix[i + 1][j - 1][DESCENDING])
                    dp_matrix[i][j][DESCENDING] = mod_add(dp_matrix[i + 1][j][DESCENDING], dp_matrix[i + 1][j - 1][DESCENDING], MODULE);
                else 
                    dp_matrix[i][j][DESCENDING] = mod_add(dp_matrix[i + 1][j][DESCENDING], dp_matrix[i + 1][j - 1][ASCENDING], MODULE);
                if (i >= j)
                    dp_matrix[i][j][DESCENDING] = mod_add(dp_matrix[i][j][DESCENDING], dp_matrix[i][j][ASCENDING], MODULE);
                if (i + 1 >= j)
                    dp_matrix[i][j][DESCENDING] = mod_sub(dp_matrix[i][j][DESCENDING], dp_matrix[i + 1][j][ASCENDING], MODULE);
                if (i + h < depth) {
                    if (dp_matrix[i + h][j - 1][DESCENDING])
                        dp_matrix[i][j][DESCENDING] = mod_sub(dp_matrix[i][j][DESCENDING], dp_matrix[i + h][j - 1][DESCENDING], MODULE);
                    else
                        dp_matrix[i][j][DESCENDING] = mod_sub(dp_matrix[i][j][DESCENDING], dp_matrix[i + h][j - 1][ASCENDING], MODULE);
                }
                if (i == 0) break;
            }
        }
    }

    // Sum
    for (j = 2; j < n; ++j) {
        sum = mod_add(sum, dp_matrix[0][j][DESCENDING], MODULE);
    }
    return sum;

}

int main(void) {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	std::size_t depth, tests, n, h, H;

    std::cin >> tests;
    while(tests--) {
        std::cin >> n >> h >> H;
        if (H < h || h < 2) {
            std::cout << "0\n";
            continue;
        }
        depth = std::min(H - h, (h - 1) * (n - 1 - (n / 2))) + 1;
        for (std::size_t i = 0; i < depth; ++i){
            for(std::size_t j = 0; j < n; ++j){
                std::fill(dp_matrix[i][j].begin(), dp_matrix[i][j].end(), 0);
            }
        }
        std::cout << solve(h, n, depth) << "\n";
    }
	return EXIT_SUCCESS;
}