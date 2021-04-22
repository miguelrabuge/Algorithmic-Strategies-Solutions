#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>

#define MODULE 1000000007

int mod_abs(int a, int mod) {
	return ((a % mod) + mod) % mod;
}

int mod_add(int a, int b, int mod) {
	return (mod_abs(a, mod) + mod_abs(b, mod)) % mod;
}

int mod_sub(int a, int b, int mod) { 
	return mod_add(a, -b, mod);
}

int solve(std::size_t &h, std::size_t n) {
    if (n == 3) {
        return h - 1;
    } else if (n % 2 == 0) {
        int value = solve(h, n - 1); 
        int sum = 0;
        for (int i = 0; i < h - 1; i++)
            sum = mod_add(value, sum, MODULE);
        return sum;
    } else {
        int value = solve(h, n - 1); 
        int sum = 0;
        for (int i = 0; i < h - 1; i++)
            sum = mod_add(value, sum, MODULE);
        return mod_add((h - 1),sum, MODULE);
    }
}

int main(void) {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	std::size_t tests, n, h, H;

    std::cin >> tests;
    while(tests--) {
        std::cin >> n >> h >> H;
        std::cout << solve(h, n) % MODULE << "\n";
    }
	return EXIT_SUCCESS;
}