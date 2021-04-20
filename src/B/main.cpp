#include <iostream>
#include <vector>
#include <numeric>

#define MODULE 1000000007

template <typename T>
using Matrix = std::vector<std::vector<T>>;

void debug(Matrix<int> &dp_matrix) {
	for (size_t i = 0; i < dp_matrix.size(); i++) {
		for (size_t j = 0; j < dp_matrix[0].size(); j++) {
			std::cout << dp_matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

int mod_abs(int a, int mod) {
	return ((a % mod) + mod) % mod;
}

int mod_add(int a, int b, int mod) {
	return (mod_abs(a, mod) + mod_abs(b, mod)) % mod;
}

int mod_sub(int a, int b, int mod) { 
	return mod_add(a, -b, mod);
}

int solve(Matrix<int> &dp_matrix, unsigned int &max_width, unsigned int &block_height, unsigned int &max_height) {
	dp_matrix[0][0] = 1;
	size_t i,j;
	// Ascending Step
	for (i = 0; i < max_height - block_height + 1; ++i) {
		for (j = 0; j <= i && j < max_width; ++j) {
			if (dp_matrix[i][j] != 0) {
				for (size_t k = i + 1; k < i + block_height && k < max_height - block_height + 1; ++k) {
					dp_matrix[k][j + 1] = mod_add(dp_matrix[k][j + 1], dp_matrix[i][j], MODULE);
				}
			}
		}
	}
	// Descending Step
	for (size_t j = 2; j < max_width; ++j) {
		for (size_t i = 0; i < max_height - block_height + 1; ++i){
			for (size_t k = i + 1; k < i + block_height && k < max_height - block_height + 1; ++k){
				dp_matrix[i][j] = mod_add(dp_matrix[i][j], dp_matrix[k][j - 1], MODULE);
			}
		}
	}

	// Returning Possible Solutions
	return mod_abs(std::accumulate(dp_matrix[0].begin(), dp_matrix[0].end(), -1), MODULE);
}

int main(void) {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	unsigned int t, n, h, H;
	std::cin >> t;
	while (t--) {
		std::cin >> n >> h >> H;
		Matrix<int> dp_matrix(H - h + 1, std::vector<int>(n , 0));
		std::cout << solve(dp_matrix, n, h, H) << "\n";
	}
	return EXIT_SUCCESS;
}