#include <iostream>
#include <vector>
#include <stack>
#include <queue>

enum class QuestionType {
	NUMBER_OF_CIRCUITS,
	LARGEST_CIRCUIT_POI_COUNT,
	LONGEST_LANE,
	TOTAL_LANE_LENGTH
};

// Union Find Disjoint Set Operations
template <typename S, typename R, typename V>
void make_set(S &pset, R &rank, V const i) {
	pset[i] = i;
	rank[i] = 0;
}

template <typename Set, typename V>
V find_set(Set &pset, V const i) {
	if (pset[i] != i)
		pset[i] = find_set(pset, pset[i]);
	return pset[i];
}

template <typename S, typename R, typename V>
void union_set(S &pset, R &rank, V const i, V const j) {
	V a = find_set(pset, i);
	V b = find_set(pset, j);
	rank[a] > rank[b] ? pset[b] = a : pset[a] = b;
	if (rank[a] == rank[b])
		rank[b]++;
}

template <typename V = int, typename W = int>
class City {

	using Circuit = std::vector<V>;
	using AdjacencyList = std::vector<std::vector<std::pair<V, W>>>;

	using Edge = std::pair<W, std::pair<V, V>>;
	struct EdgeComparator {
		constexpr bool operator()(Edge const &a, Edge const &b) noexcept {
			return a.first > b.first;
		}
	};
	using EdgeList = std::priority_queue<Edge, std::vector<Edge>, EdgeComparator>;

	std::size_t m_streets;
	AdjacencyList m_graph;

	std::pair<std::size_t, V> m_longest_lane;
	std::size_t m_max_circuit_poi;
	std::size_t m_total_lane_length;
	std::vector<Circuit> m_circuits;

	void m_calculate_city_circuits() {
		std::size_t t = 0;
		std::stack<V> stack;
		std::vector<V> low(m_graph.size(), static_cast<V>(-1)), dfs(m_graph.size(), static_cast<V>(-1));
		std::vector<bool> is_stacked(m_graph.size(), false);

		for (V v = 0; v < static_cast<V>(m_graph.size()); ++v) {
			if (dfs[v] == static_cast<V>(-1)) {
				m_tarjan(v, t, dfs, low, stack, is_stacked);
			}
		}
	}

	void m_calculate_city_lanes() {
		for (auto const &circuit : m_circuits) {
			auto ll = m_kruskal(circuit);
			if (ll > m_longest_lane.first ||
			    (ll == m_longest_lane.first && m_longest_lane.second < circuit.size())) {
				m_longest_lane = std::make_pair(ll, circuit.size());
			}
			m_total_lane_length += ll;
		}
	}

	void m_tarjan(V const v, V &t, std::vector<V> &dfs, std::vector<V> &low,
		      std::stack<V> &stack, std::vector<bool> &is_stacked) {
		low[v] = dfs[v] = t++;
		stack.push(v);
		is_stacked[v] = true;

		for (auto const &[w, _] : m_graph[v]) {
			if (dfs[w] == static_cast<V>(-1)) {
				m_tarjan(w, t, dfs, low, stack, is_stacked);
				low[v] = std::min(low[v], low[w]);
			} else if (is_stacked[w]) {
				low[v] = std::min(low[v], dfs[w]);
			}
		}

		if (low[v] == dfs[v]) {
			V w;
			std::vector<V> circuit;
			do {
				w = stack.top();
				stack.pop();
				is_stacked[w] = false;
				circuit.push_back(w);
			} while (w != v);

			if (circuit.size() > 1) {
				m_max_circuit_poi = std::max(m_max_circuit_poi, circuit.size());
				m_circuits.emplace_back(std::move(circuit));
			}
		}
	}

	std::size_t m_kruskal(Circuit const &circuit) {
		EdgeList edgelist;
		std::vector<V> pset(m_graph.size(), static_cast<V>(-1));
		std::vector<std::size_t> rank(m_graph.size());

		for (auto const &s : circuit) {
			make_set(pset, rank, s);
		}

		for (auto const &s : circuit) {
			for (auto const &[t, w] : m_graph[s]) {
				if (pset[t] != static_cast<V>(-1)) {
					edgelist.push({w, {s, t}});
				}
			}
		}

		std::size_t lane_length = 0;
		while (!edgelist.empty()) {
			Edge edge = edgelist.top();
			edgelist.pop();
			if (find_set(pset, edge.second.first) != find_set(pset, edge.second.second)) {
				lane_length += static_cast<std::size_t>(edge.first);
				union_set(pset, rank, edge.second.first, edge.second.second);
			}
		}
		return lane_length;
	}

public:
	City(V const points_of_interest, std::size_t const streets)
	    : m_streets(streets),
	      m_graph(points_of_interest),
	      m_longest_lane({0, 0}),
	      m_max_circuit_poi(0),
	      m_total_lane_length(0) {
		for (V i = 0; i < points_of_interest; ++i)
			m_graph[i].reserve(points_of_interest);
		m_circuits.reserve(points_of_interest);
	}

	constexpr std::size_t circuit_count() const { return m_circuits.size(); }
	constexpr std::size_t max_circuit_poi() const { return m_max_circuit_poi; }
	constexpr std::size_t longest_lane() const { return m_longest_lane.first; }
	constexpr std::size_t total_lane_length() const { return m_total_lane_length; }

	void answer(std::size_t const questions) {
		QuestionType n_questions = static_cast<QuestionType>(questions);
		QuestionType question = QuestionType::NUMBER_OF_CIRCUITS;

		while (question < n_questions) {
			switch (question) {
				case QuestionType::NUMBER_OF_CIRCUITS:
					m_calculate_city_circuits();
					std::cout << circuit_count();
					break;
				case QuestionType::LARGEST_CIRCUIT_POI_COUNT:
					std::cout << max_circuit_poi();
					break;
				case QuestionType::LONGEST_LANE:
					m_calculate_city_lanes();
					std::cout << longest_lane();
					break;
				case QuestionType::TOTAL_LANE_LENGTH:
					std::cout << total_lane_length();
					break;
			}
			question = static_cast<QuestionType>(static_cast<std::size_t>(question) + 1);
			if (question != n_questions)
				std::cout << " ";
		}
		std::cout << "\n";
	}

	friend std::istream &operator>>(std::istream &is, City &city) {
		W distance;
		V source, target;
		for (std::size_t i = 0; i < city.m_streets; ++i) {
			is >> source >> target >> distance;
			city.m_graph[source - 1].emplace_back(target - 1, distance);
		}
		return is;
	}

	friend std::ostream &operator<<(std::ostream &os, City const &city) {
		for (std::size_t i = 0; i < city.m_graph.size(); ++i) {
			os << static_cast<char>('A' + i) << "[" << i << "]"
			   << ": ";
			for (auto &[v, w] : city.m_graph[i]) {
				os << "(" << static_cast<char>('A' + v)
				   << "[" << v << "]"
				   << ", " << w << ")"
				   << " ";
			}
			std::cerr << "\n";
		}
		return os;
	}
};

int main(void) {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	std::size_t tt, n, m, q;

	std::cin >> tt;
	while (tt--) {
		std::cin >> n >> m >> q;
		City city(n, m);
		std::cin >> city;
		city.answer(q);
	}
	return EXIT_SUCCESS;
}