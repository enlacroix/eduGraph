#pragma once
#include <unordered_map>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include "matrix/matrix.h"

namespace graph {
	//  вводим шаблонные псевдонимы для пр-ва имен?
	// Блок Исключений
	struct GraphException : public std::runtime_error {
		GraphException(const char* message) : runtime_error(message) {}
	};

	struct GraphKeyError : public GraphException {
		GraphKeyError() : GraphException("Passed key doesn't exist!") {}
	};

	struct InvalidAdjMatrix : public GraphException {
		InvalidAdjMatrix() : GraphException("The adjacency matrix obtained in the file is written incorrectly!") {}
	};

	struct InvalidPath : public GraphException {
		InvalidPath() : GraphException("Can't find file by the specified path.") {}
	};

	struct ExpectedAnotherSymbol : public GraphException {
		ExpectedAnotherSymbol() : GraphException("Wrong symbol, expected another one.") {}
	};

	struct AllElementsMustBePositive : public GraphException {
		AllElementsMustBePositive() : GraphException("All elements in adj matrix have to be positive, requirement for Dijkstra algorithm.") {}
	};

	struct InvalidInputArgs : public GraphException {
		InvalidInputArgs() : GraphException("Input arguments are wrong, try again!") {}
	};

	struct NoRoute : public GraphException {
		NoRoute() : GraphException("It is impossible to build a route between these two nodes! ") {}
	};

	template<typename key_type, typename value_type, typename weight_type>
	class Graph {
	public:
		using alias_key_type = key_type;
		using alias_value_type = value_type;
		using alias_weight_type = weight_type;
		// Этот блок выглядит чуть-чуть странно???

		class Node;
		using grmap = typename std::unordered_map <key_type, Node>;
		using const_iterator = typename grmap::const_iterator;
		using iterator = typename grmap::iterator;
		std::string graphName = "My graph";
		
		iterator begin() { return m_graph.begin(); }
		iterator end() { return m_graph.end(); }
		const_iterator cbegin() const { return m_graph.cbegin(); }
		const_iterator cend() const { return m_graph.cend(); }

		Graph() = default;
		Graph(const Graph& other) { m_graph = other.getGraphMap(); };
		Graph(Graph&& other) noexcept { this->swap(other);  };
		Graph(linalg::Matrix<weight_type>& adj);
		Graph& operator= (const Graph& other) noexcept {
			m_graph = other.getGraphMap();
			return *this;
		};
		Graph& operator= (Graph&& other) noexcept {
			m_graph.swap(other.getGraphMap());
			return *this;
		};

		bool empty() const { return m_graph.empty(); }
		size_t size() const { return m_graph.size(); }
		void clear() { m_graph.clear(); }
		void swap(Graph<key_type, value_type, weight_type>&);
		void show() const; // избыточно в интерфейсе
		void showLabels() const;
		void setName(std::string newgraphName) { graphName = newgraphName; };

		grmap& getGraphMap() { return m_graph; }; // было: grmap& getGraphMap() { return m_graph; };
		grmap getGraphMap() const { return m_graph; };

		Node& operator[](key_type const key) { return m_graph[key]; }
		Node& at(key_type const key) { return m_graph.at(key); }

		// Обратимся ко второму элементу пары (Node) и запрашиваем у него метод size(), который определен ниже.
		size_t degree_in(key_type key) const;
		size_t degree_out(key_type key) const { return m_graph.find(key)->second.size(); }; // вот здесь всё не так просто, однострочник не выйдет(
		bool loop(key_type) const;

		// Мы передаем список инициализации именно в insert
		std::pair<iterator, bool> insert_node(key_type key, value_type value) {return m_graph.insert({ key, value });}
		std::pair<iterator, bool> insert_or_assign_node(key_type key, value_type value) {
			return m_graph.insert_or_assign(key, value);
		}
		// ключ откуда, ключ куда, вес ребра. 
		std::pair<typename Node::iterator, bool> insert_edge(std::pair<key_type, key_type>, weight_type);
		std::pair<typename Node::iterator, bool> insert_or_assign_edge(std::pair<key_type, key_type>, weight_type);

		// Экспериментальная часть: удаление
		void deleteNode(key_type key);
		void deleteEdge(std::pair<key_type, key_type> keyPair);

		linalg::Matrix<weight_type> getAdjacencyMatrix(bool adjFlag = true);
		linalg::Matrix<int> getReachabilityMatrix();


	private:
		grmap m_graph;

	};
	
	template<typename key_type, typename value_type, typename weight_type>
	class Graph<key_type, value_type, weight_type>::Node {
	public:
		using nodemap = typename std::unordered_map<key_type, weight_type>; 

		bool isVisited = false; // для Дейкстры ИСПРАВИЛ ДЛЯ ЭКСПЕРИМЕНТА - ПОПРАВИТЬ обратно на false!
		weight_type label = 0; // для Дейкстры, начальная метка
		key_type parent; 

		Node() = default;
		Node(value_type value) : m_value(value) {};
		Node(const Node& other) {
			m_value = other.value();
			m_edges = other.getEdgesMap();
		};
		Node(Node&& other) noexcept { 
			m_value = other.value(); // FIX
			m_edges.swap(other.getEdgesMap());
		};
		Node& operator= (const Node& other) noexcept {
			m_value = other.value();
			m_edges = other.getEdgesMap();
			return *this;
		};
		Node& operator= (Node&& other) noexcept {
			m_value = other.value();
			m_edges.swap(other.getEdgesMap());
			return *this;
		};
		
		using const_iterator = typename nodemap::const_iterator;
		using iterator = typename nodemap::iterator;

		iterator begin() { return m_edges.begin(); }
		iterator end() { return m_edges.end(); }
		const_iterator cbegin() const { return m_edges.cbegin(); }
		const_iterator cend() const { return m_edges.cend(); }

		void setVisited() { isVisited = true; }
		bool empty() const { return m_edges.empty(); }
		size_t size() const { return m_edges.size(); }
		void clear() { m_edges.clear(); }
		value_type& value() { return m_value; }
		value_type value() const { return m_value; }
		void show() const;

		std::pair<Graph::Node::iterator, bool> add_edge(key_type key, weight_type weight) {
			return m_edges.emplace(key, weight);
		} 

		nodemap& getEdgesMap() { return m_edges; }
		nodemap getEdgesMap() const { return m_edges; }

	private:
		value_type m_value;
		nodemap m_edges; // Храним здесь только ИСХОДЯЩИЕ из данного узла рёбра. 
	};

template<typename key_type, typename value_type, typename weight_type>
void swap(Graph<key_type, value_type, weight_type>& one, Graph<key_type, value_type, weight_type>& another) {one.swap(another);}



}

template<typename key_type, typename value_type, typename weight_type>
graph::Graph<key_type, value_type, weight_type>::Graph(linalg::Matrix<weight_type>& adj) {
	if (!adj.square()) throw InvalidAdjMatrix();
	size_t dim = adj.rows();
	for (key_type i = 1; i <= dim; i++)
	{
		this->insert_node(i, value_type());
	}
	for (size_t rows = 1; rows <= dim; rows++) {
		for (size_t cols = 1; cols <= dim; cols++)
		{
			weight_type my_weight = adj[rows - 1][cols - 1];
			if (my_weight != 0) this->insert_edge({rows, cols}, my_weight);
		}
	}
}

template<typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::show() const {
	std::cout << this->graphName << "\n";
	std::cout << "Number of nodes: " << this->size() << "\n";
	if (this->empty()) {
		std::cout << "This graph is empty right now." << "\n";
	}
	else {
		for (auto const& pair : m_graph) {
			std::cout << pair.first << "[";
			pair.second.show();
		}
	}
};

template<typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::Node::show() const {
	std::cout << m_value << "]: "; 
	for (auto const& pair : m_edges)
		std::cout << pair.first << " {" << pair.second << "}; ";
	std::cout << "\n";
};

template<typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::showLabels() const {
	for (auto const& pair : m_graph) {
		std::cout << pair.first << "[" << pair.second.label << "]: " << pair.second.parent << ", " <<
			std::boolalpha << pair.second.isVisited << "\n";
	}	
};

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::Node::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_edge(std::pair<key_type, key_type> keyPair, weight_type weight) {
	auto iteratorStartPair = m_graph.find(keyPair.first);
	// В случае неудачного поиска методы возвращают итераторы на конец контейнера, чем мы и пользуемся. 
	if (iteratorStartPair == m_graph.end() || m_graph.find(keyPair.second) == m_graph.end()) {
		std::cout << "Key wasn't found!" << "\n"; // это для себя, в "релиз" не пойдёт
		return { iteratorStartPair->second.begin(), false }; // обращаемся именно к Node, и возвращаем именно её итератор.
	}
	return iteratorStartPair->second.add_edge(keyPair.second, weight);
}

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::Node::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_or_assign_edge(std::pair<key_type, key_type> keyPair, weight_type weight) {
	auto iteratorStartPair = m_graph.find(keyPair.first);
	if (iteratorStartPair == m_graph.end() || m_graph.find(keyPair.second) == m_graph.end()) {
		std::cout << "Key wasn't found!" << "\n";
		return { iteratorStartPair->second.begin(), false };
	}
	auto& edgeMapOfFirstNode = iteratorStartPair->second.getEdgesMap(); // добавил ссылку и все заработало. 
	for (const auto& elem : edgeMapOfFirstNode) // для убеждения в том, что обрабатываем корректуню мапу
	{
		std::cout << elem.first << " " << elem.second << "\n";
	}
	return edgeMapOfFirstNode.insert_or_assign(keyPair.second, weight);
}


template<typename key_type, typename value_type, typename weight_type>
size_t graph::Graph<key_type, value_type, weight_type>::degree_in(key_type key) const {
	size_t edgesOut = 0;
	for (auto const& pair : m_graph) {
		if (pair.first != key) { // Если рассматриваемая пара не наша собственная (ниже я сразу же этим приёмом найду петли)
			auto xnode = pair.second;
			auto edgeMap = xnode.getEdgesMap();
			if (edgeMap.find(key) != edgeMap.end()) { // если в карте ноды нашёлся искомый ключ, то увеличиваем
				++edgesOut;
			}
		}
	}
	return edgesOut;
}

template<typename key_type, typename value_type, typename weight_type>
bool graph::Graph<key_type, value_type, weight_type>::loop(key_type key) const {
	if (!m_graph.count(key)) {
		throw GraphKeyError();
		return false;
	}
	auto myNode = m_graph.find(key)->second;
	auto edgeMap = myNode.getEdgesMap();
	for (auto const& pair : edgeMap)
		if (pair.first == key) return true;
	return false;
}

template<typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::swap(Graph<key_type, value_type, weight_type>& other) {
	m_graph.swap(other.m_graph);
}

template <typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::deleteNode(key_type key)
{
	for (auto it = m_graph.begin(); it != m_graph.end(); it++)
		// Итерируемся по нодам графа, у каждой пытаемся стереть искомый ключ из их собственного ресурса edges. 
		// Не забыть удалить из самой мапы графа ключ. 
	{
		(((*it).second).getEdgesMap()).erase(key);
	}
	m_graph.erase(key);
}

template <typename key_type, typename value_type, typename weight_type>
void graph::Graph<key_type, value_type, weight_type>::deleteEdge(std::pair<key_type, key_type> keyPair)
{
	if (!m_graph.count(keyPair.first)) throw GraphKeyError();
	(m_graph[keyPair.first].getEdgesMap()).erase(keyPair.second);
}

template <typename key_type, typename value_type, typename weight_type>
linalg::Matrix<weight_type> graph::Graph<key_type, value_type, weight_type>::getAdjacencyMatrix(bool adjFlag) {
	size_t numberOfNodes = this->size();
	linalg::Matrix<int> m(numberOfNodes, numberOfNodes, 0);
	std::unordered_map<key_type, int> enumerationOfKeys; 
	size_t i = 0;
	for (auto const& pairKeyNode : m_graph) {
		enumerationOfKeys.insert({ pairKeyNode.first, i });
		i++;
	}
	for (auto const& pairKeyNode : m_graph) {
		for (auto const& pairKeyWeight : m_graph[pairKeyNode.first].getEdgesMap()) {
			weight_type val; 
			if (adjFlag) val = 1;
			else val = pairKeyWeight.second;
			m[enumerationOfKeys[pairKeyNode.first]][enumerationOfKeys[pairKeyWeight.first]] = val;
		}
	}
	return m;
}

template <typename key_type, typename value_type, typename weight_type>
linalg::Matrix<int> graph::Graph<key_type, value_type, weight_type>::getReachabilityMatrix() {
	size_t numberOfNodes = this->size();
	auto adj = this->getAdjacencyMatrix();
	auto m = linalg::eye<int>(numberOfNodes) + adj;
	for (int i = 2; i <= numberOfNodes - 1; i++) {
		m += adj.power(i);
	}
	return m;
}

