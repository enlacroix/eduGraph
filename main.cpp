#include "dijkstra.h"


// int arg_count, char* arg_vars[]
int main(int arg_count, char* arg_vars[]) {
	// setlocale(LC_ALL, "ru");
	
	
	/*
	* linalg::Matrix adj = { {0, 5, 1, 0}, {0, 0, 0, 0}, {0, 6, 0, 1}, {0, 0, 5, 0} };
	Graph<std::size_t, std::string, double> mySummerGraph(adj);
	mySummerGraph.setName("Prekrasnoye daleko");
	std::cout << mySummerGraph.getReachabilityMatrix() << "\n";
	mySummerGraph.insert_node(1, "Строгино"); 
	mySummerGraph.insert_node(2, "Варшавская");
	mySummerGraph.insert_node(3, "Площадь Революции");
	mySummerGraph.insert_node(4, "Ленинский проспект");
	mySummerGraph.insert_node(5, "Автозаводская");
	mySummerGraph.insert_node(6, "Купчино");

	mySummerGraph.insert_edge(std::make_pair(1, 2), 60);
	mySummerGraph.insert_edge(std::make_pair(1, 3), 20);
	mySummerGraph.insert_edge(std::make_pair(1, 1), 10); 
	mySummerGraph.insert_edge(std::make_pair(2, 4), 45);
	mySummerGraph.insert_edge(std::make_pair(2, 5), 15);
	mySummerGraph.insert_edge(std::make_pair(4, 5), 40);
	mySummerGraph.insert_edge(std::make_pair(3, 5), 5);
	mySummerGraph.insert_edge(std::make_pair(5, 4), 5);
	mySummerGraph.insert_edge(std::make_pair(3, 6), 185.5);
	mySummerGraph.insert_edge(std::make_pair(3, 3), 20);
	mySummerGraph.show();
	std::cout << mySummerGraph.getAdjacencyMatrix(false);
	*/
	
	auto [filepath, key_from, key_to] = alg::parse_args(arg_count, arg_vars);
	alg::graph_t graph = alg::read_graph(filepath);
	std::cout << graph.getAdjacencyMatrix(false) << "\n";
	std::cout << graph.getReachabilityMatrix() << "\n";
	auto [weight, route] = alg::dijkstraMain(graph, key_from, key_to);
	alg::print_results(weight, route);
	alg::visualize(filepath, route);
	
	return 0;
	
}
