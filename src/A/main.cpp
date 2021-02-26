#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

std::size_t sum = 0;
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
	TileBoard(std::size_t const size)
	    : m_matrix(size, std::vector<int>(size, 0)),
	      m_moves(0),
	      m_tiles(0) {}

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

	bool is_solvable() { return (sum & (sum - 1)) == 0; }

	bool operator==(TileBoard const &other) {
		for (std::size_t i = 0; i < m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < m_matrix.size(); ++j) {
				if (m_matrix[i][j] != other.m_matrix[i][j])
					return false;
			}
		}
		return true;
	}

	bool horizontally_equivalent_to(TileBoard const &other) {
		for (std::size_t i = 0; i < m_matrix.size(); ++i) {
			for (std::size_t j = 0, k = 0; j < m_matrix.size() && k < m_matrix.size();) {
				if (m_matrix[i][j] == 0)
					++j;
				else if (other.m_matrix[i][k] == 0)
					++k;
				else if (m_matrix[i][j] != other.m_matrix[i][k])
					return false;
				else
					++j, ++k;
			}
		}
		return true;
	}

	bool vertically_equivalent_to(TileBoard const &other) {
		for (std::size_t i = 0, j = 0; i < m_matrix.size() && j < m_matrix.size();) {
			for (std::size_t k = 0; k < m_matrix.size(); ++k) {
				if (m_matrix[k][i] == 0)
					++i;
				else if (other.m_matrix[k][j] == 0)
					++j;
				else if (m_matrix[k][i] != other.m_matrix[k][j])
					return false;
				else
					++i, ++j;
			}
		}
		return true;
	}

	friend std::istream &operator>>(std::istream &is, TileBoard &board) {
		sum = 0;
		for (std::size_t i = 0; i < board.m_matrix.size(); ++i) {
			for (std::size_t j = 0; j < board.m_matrix.size(); ++j) {
				std::cin >> board.m_matrix[i][j];
				if (board.m_matrix[i][j])
					++board.m_tiles;
				sum += static_cast<std::size_t>(board.m_matrix[i][j]);
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
	int m_best_solution;

	bool backtrack(TileBoard board, SwipeAction action, SwipeAction last_action) {

		if (board.tile_count() == 1){
			m_best_solution = std::min(board.move_count(), m_best_solution);
			return true;
		}

		// moves == max_moves_allowed_by_the_problem_statement
		if (board.move_count() == m_max_moves) {
			return false;
		}

		if (board.move_count() == m_best_solution){
			return false;
		}

		TileBoard saved = board;
		if (action == SwipeAction::SWIPE_LEFT)
			board.swipe_left();
		else if (action == SwipeAction::SWIPE_RIGHT)
			board.swipe_right();
		else if (action == SwipeAction::SWIPE_UP)
			board.swipe_up();
		else if (action == SwipeAction::SWIPE_DOWN)
			board.swipe_down();

		if (action != SwipeAction::NONE && board == saved) {
			return false;
		}

		if (((last_action == SwipeAction::SWIPE_LEFT && action == SwipeAction::SWIPE_RIGHT) ||
		     (last_action == SwipeAction::SWIPE_RIGHT && action == SwipeAction::SWIPE_LEFT)) &&
		    board.horizontally_equivalent_to(saved)) {
			return false;
		}

		if (((last_action == SwipeAction::SWIPE_UP && action == SwipeAction::SWIPE_DOWN) ||
		     (last_action == SwipeAction::SWIPE_DOWN && action == SwipeAction::SWIPE_UP)) &&
		    board.vertically_equivalent_to(saved)) {
			return false;
		}

		last_action = action;
		
		
		return backtrack(board, SwipeAction::SWIPE_LEFT, last_action) |
		       backtrack(board, SwipeAction::SWIPE_RIGHT, last_action) |
		       backtrack(board, SwipeAction::SWIPE_UP, last_action) |
		       backtrack(board, SwipeAction::SWIPE_DOWN, last_action);
	}

public:
	GameSolver(int max_moves)
	    : m_max_moves(max_moves),
	      m_best_solution(max_moves) {}

	int solve(TileBoard &board) {
		return board.is_solvable() && backtrack(board, SwipeAction::NONE, SwipeAction::NONE)
			   ? m_best_solution
			   : -1;
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
		auto answer = solver.solve(board);
		if (answer != -1)
			std::cout << answer << "\n";
		else
			std::cout << "no solution\n";
	}
	return EXIT_SUCCESS;
}