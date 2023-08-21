#pragma once
#include "graph.h" // graph_lw3\graph.h
#include <limits> // для определения метки "бесконечности"
#include <algorithm> 
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>


using namespace graph;

namespace alg {
	using node_name_t = typename std::size_t;
	using node_data_t = typename std::string;
	using weight_t = double;
	using graph_t = typename Graph<node_name_t, node_data_t, weight_t>;
	using route_t = std::vector<node_name_t>;

	const weight_t INF = std::numeric_limits<weight_t>::infinity(); // было max() до беседы в тг

	
	std::tuple<const char*, node_name_t, node_name_t> parse_args(int arg_count, char* arg_vars[]);

	graph_t read_graph(const char* file_name);
	size_t getNumberofNodes(std::ifstream& file);

	std::pair<weight_t, route_t> dijkstraMain(graph_t& graph, node_name_t key_from, node_name_t key_to);
	node_name_t getNextKey(graph_t& graph);
	void print_results(weight_t, const route_t&);

	void visualize(const char* filePath, route_t& route);

	
}
