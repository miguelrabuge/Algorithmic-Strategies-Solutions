#include <algorithm>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

using Matrix = std::vector<std::vector<int>>;

enum class SwipeAction {
	SWIPE_LEFT,
	SWIPE_RIGHT,
	SWIPE_UP,
	SWIPE_DOWN,
	NONE,
};

class TileBoard {
private:
	Matrix m_matrix;
	int m_moves;
	int m_tiles;

	template <typename RandomIter>
	void horizontal_swipe(RandomIter first, RandomIter last) {
		for (auto i = first, j = (first + 1); j != last; ++j) {
			if ((*j) == 0)
				continue;
			if ((*i) == 0)
				(*i) += (*j), (*j) = 0;
			else if ((*i) == (*j))
				(*i) += (*j), (*j) = 0, ++i, --m_tiles;
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
				(*i)[column] += (*j)[column], (*j)[column] = 0, ++i, --m_tiles;
			else if (i + 1 < j)
				++i, --j;
			else
				++i;
		}
	}

public:
	TileBoard(std::size_t const size) : m_matrix(size, std::vector<int>(size, 0)), m_moves(0), m_tiles(0) {}

	int move_count() const {
		return m_moves;
	}

	int tile_count() const {
		return m_tiles;
	}

	void swipe_left() {
		for (auto &row : m_matrix) {
			TileBoard::horizontal_swipe(row.begin(), row.end());
		}
		++m_moves;
	}

	void swipe_right() {
		for (auto &row : m_matrix) {
			TileBoard::horizontal_swipe(row.rbegin(), row.rend());
		}
		++m_moves;
	}

	void swipe_up() {
		for (std::size_t col = 0; col < m_matrix.size(); ++col) {
			TileBoard::vertical_swipe(m_matrix.begin(), m_matrix.end(), col);
		}
		++m_moves;
	}

	void swipe_down() {
		for (std::size_t col = 0; col < m_matrix.size(); ++col) {
			TileBoard::vertical_swipe(m_matrix.rbegin(), m_matrix.rend(), col);
		}
		++m_moves;
	}

	friend std::istream &operator>>(std::istream &is, TileBoard &board) {
		for (std::size_t i = 0; i < board.m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < board.m_matrix.size(); ++j) {
				std::cin >> board.m_matrix[i][j];
				if (board.m_matrix[i][j])
					++board.m_tiles;
			}
		}
		return is;
	}

	friend std::ostream &operator<<(std::ostream &os, TileBoard &board) {
		for (std::size_t i = 0; i < board.m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < board.m_matrix.size(); ++j) {
				os << board.m_matrix[i][j] << " ";
			}
			os << "\n";
		}
		return os;
	}
};

class GameSolver {
	int m_max_moves;
	int m_best;
	std::unordered_map<int, int> m_memo;

	bool backtrack(TileBoard board, SwipeAction action) {

		if (board.move_count() == m_max_moves)
			return false;

		if (board.tile_count() == 1) {
			m_memo[board.move_count()] = board.tile_count();
			m_best = std::min(board.move_count(), m_best);
			return true;
		}

		if (action == SwipeAction::SWIPE_LEFT)
			board.swipe_left();
		else if (action == SwipeAction::SWIPE_RIGHT)
			board.swipe_right();
		else if (action == SwipeAction::SWIPE_UP)
			board.swipe_up();
		else if (action == SwipeAction::SWIPE_DOWN)
			board.swipe_down();

		if (!m_memo[board.move_count()])
			m_memo[board.move_count()] = std::numeric_limits<int>::max();

		if (board.tile_count() > m_memo[board.move_count()])
			return false;
		m_memo[board.move_count()] = board.tile_count();

		return backtrack(board, SwipeAction::SWIPE_LEFT) |
		       backtrack(board, SwipeAction::SWIPE_RIGHT) |
		       backtrack(board, SwipeAction::SWIPE_UP) |
		       backtrack(board, SwipeAction::SWIPE_DOWN);
	}

public:
	GameSolver(int max_moves) : m_max_moves(max_moves), m_best(max_moves) {}

	std::string solve(TileBoard &board) {
		m_memo[board.move_count()] = board.tile_count();
		return backtrack(board, SwipeAction::NONE)
			   ? std::to_string(m_best)
			   : "no solution";
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
		GameSolver solver(max_moves);
		TileBoard board(size);
		std::cin >> board;
		std::cout << solver.solve(board) << "\n";
	}
	return EXIT_SUCCESS;
}