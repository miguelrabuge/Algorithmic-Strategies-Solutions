#include <algorithm>
#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

class Board {
	Matrix m_matrix;
	int m_moves;
	int m_max_moves;

	template <typename RandomIter>
	void horizontal_swipe(RandomIter first, RandomIter last) {
		for (auto i = first, j = (first + 1); j != last; ++j) {
			if ((*j) == 0)
				continue;
			if ((*i) == 0)
				(*i) += (*j), (*j) = 0;
			else if ((*i) == (*j))
				(*i) += (*j), (*j) = 0, ++i;
			else if (i + 1 < j)
				++i, --j;
			else
				++i;
		}
	}

	template <typename RandomIter>
	void vertical_swipe(RandomIter first, RandomIter last, std::size_t column) {
		for (auto i = first, j = (first + 1); j != last; ++j) {
			if ((*j)[column] == 0)
				continue;
			if ((*i)[column] == 0)
				(*i)[column] += (*j)[column], (*j)[column] = 0;
			else if ((*i)[column] == (*j)[column])
				(*i)[column] += (*j)[column], (*j)[column] = 0, ++i;
			else if (i + 1 < j)
				++i, --j;
			else
				++i;
		}
	}

public:
	Board(std::size_t const size, int const max_moves) : m_matrix(size, std::vector<int>(size, 0)), m_moves(0), m_max_moves(max_moves) {}

	inline void swipe_left() {
		for (auto &row : m_matrix)
			Board::horizontal_swipe(row.begin(), row.end());
		++m_moves;
	}

	inline void swipe_right() {
		for (auto &row : m_matrix)
			Board::horizontal_swipe(row.rbegin(), row.rend());
		++m_moves;
	}

	inline void swipe_up() {
		for (std::size_t col = 0; col < m_matrix.size(); ++col) {
			Board::vertical_swipe(m_matrix.begin(), m_matrix.end(), col);
		}
		++m_moves;
	}

	inline void swipe_down() {
		for (std::size_t col = 0; col < m_matrix.size(); ++col) {
			Board::vertical_swipe(m_matrix.rbegin(), m_matrix.rend(), col);
		}
		++m_moves;
	}

	inline friend std::istream &operator>>(std::istream &is, Board &board) {
		for (std::size_t i = 0; i < board.m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < board.m_matrix.size(); ++j) {
				std::cin >> board.m_matrix[i][j];
			}
		}
		return is;
	}

	inline friend std::ostream &operator<<(std::ostream &os, Board &board) {
		for (std::size_t i = 0; i < board.m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < board.m_matrix.size(); ++j) {
				os << board.m_matrix[i][j] << " ";
			}
			os << "\n";
		}
		return os;
	}
};

int main(void) {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int max_moves, tests;
	std::size_t size;

	std::cin >> tests;
	while (tests--) {
		std::cin >> size >> max_moves;
		Board board(size, max_moves);
		std::cin >> board;

		// ! Use std::cerr stream for debugging purposes

		// std::cerr << "Before Swipe: \n" << board << '\n';
		// board.swipe_left();
		// std::cerr << "After Swipe: \n" <<  board;
		// std::cerr << "=================================\n";
	}

	return EXIT_SUCCESS;
}