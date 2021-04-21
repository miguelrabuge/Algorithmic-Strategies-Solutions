#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>

#define MODULE 1000000007

std::array<std::array<int, 500>, 60000> dp_matrix;

int mod_abs(int a, int mod) {
	return ((a % mod) + mod) % mod;
}

int mod_add(int a, int b, int mod) {
	return (mod_abs(a, mod) + mod_abs(b, mod)) % mod;
}

int mod_sub(int a, int b, int mod) { 
	return mod_add(a, -b, mod);
}

int solve(std::size_t &h, std::size_t &n, std::size_t &depth) {
    std::size_t i, j, k;
    int sum = 0;
    dp_matrix[0][0] = 1;

    // Ascending Step
    for (j = 0; j < n - 1; ++j) {
        for (i = j; (i < depth) && dp_matrix[i][j]; ++i) {
            for (k = i + 1; (k < i + h) && (k < depth); ++k) {
                dp_matrix[k][j + 1] = mod_add(dp_matrix[k][j + 1], dp_matrix[i][j], MODULE);
            }
        }
    }

    // Descending Step
    for (j = 2; j < n; ++j) {
        for (i = 0; i < depth; ++i) {
            for (k = i + 1; (k < i + h) && (k < depth); ++k) {
                dp_matrix[i][j] = mod_add(dp_matrix[i][j], dp_matrix[k][j - 1], MODULE);
            }
        }
    }

    // Sum
    for (j = 2; j < n; ++j) {
        sum = mod_add(sum, dp_matrix[0][j], MODULE);
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
        depth = std::min(H - h, (h - 1) * (n - 1 - (n / 2))) + 1;
        for (std::size_t i = 0; i < 60000; ++i){
            std::fill(dp_matrix[i].begin(), dp_matrix[i].end(), 0);
        }
        std::cout << solve(h, n, depth) << "\n";
    }
	return EXIT_SUCCESS;
}