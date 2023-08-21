#include "dijkstra.h"
using namespace alg;

bool CompareLabels(graph_t::iterator pair1, graph_t::iterator pair2) { return pair1->second.label < pair2->second.label; }
bool UnvistitedAndMinimalNode(graph_t::iterator pair, weight_t minimum) {
    return pair->second.label == minimum && pair->second.isVisited == false;
}

std::pair<weight_t, route_t> alg::dijkstraMain(graph_t& gr, node_name_t start, node_name_t finish)
{   
    if (start == finish) return { 0, std::vector<node_name_t>{} };
    try {
        gr.at(start);
        gr.at(finish);
    }
    catch (...) {
        std::cerr << "This key doesn't exist!";
        exit(0);
    }
    for (auto it = gr.begin(); it != gr.end(); ++it) {
        it->second.label = alg::INF;
    }

    gr.at(start).label = 0; // ����������� ��������� ������� ����� 0 + ������ ��������
    node_name_t current_node = start;

    for (size_t i = 1; i <= gr.size(); i++) 
    {
        for (auto& edge : gr[current_node].getEdgesMap()) // �������� ����(����): ��� �����. 
        {
            if (gr[edge.first].isVisited == false && edge.second != 0 && gr[current_node].label + edge.second < gr[edge.first].label)
            {
                gr[edge.first].label = gr[current_node].label + edge.second;
                gr[edge.first].parent = current_node;
            }

        }
        gr[current_node].isVisited = true; 
        // gr.showLabels(); // ����� ��� ������. 
        if (i != gr.size()) current_node = getNextKey(gr);
        if (current_node == -1 && !(gr[finish].isVisited))
        {
            std::cerr << graph::NoRoute().what();
            exit(0);
        }
    }

    std::vector<node_name_t> route;
    node_name_t currentNode = finish;
    route.push_back(finish);
    while (currentNode != start)
    {
        currentNode = gr[currentNode].parent;
        route.push_back(currentNode);
    }
    // route.push_back(start);
    std::reverse(route.begin(), route.end());

    return {gr[finish].label, route};
}

node_name_t alg::getNextKey(graph_t& gr)
{
    node_name_t nextKey = 0; 
    bool flagNodeExists = false;
    // ��� 1. �������� ���� �� � ���, ��� ���������� ������������ ���� � ������� ������ ��������� � ������������� ������. 
    for (auto& pairKeyNode : gr.getGraphMap())
    {
        // std::cout << pairKeyNode.first << ", " << pairKeyNode.second.isVisited << "\n"; // ������� ������
        if (pairKeyNode.second.isVisited == false && pairKeyNode.second.label < INF) //������ ������� ��������� ��� ������� � ������� ������ �������!
        {
            nextKey = pairKeyNode.first;
            flagNodeExists = true;
            break;

        }
    }
    // ��� 2. ���� �������, ���� ��������� �����. 
    for (auto& pairKeyNode : gr.getGraphMap())
    {

        if (gr[nextKey].label > pairKeyNode.second.label && pairKeyNode.second.isVisited == false) nextKey = pairKeyNode.first;

    }
   // �������� ����
    if (!flagNodeExists) {
        // std::cout << "there is no route" << "\n";
        return -1;
    }
    return nextKey;
}

void alg::print_results(weight_t weight, const route_t& route) {
    std::cout << "weight: " << weight << std::endl;
    std::cout << "route: ";
    for (auto &elem: route)
    {
        std::cout << elem << ' ';
    }
}

size_t alg::getNumberofNodes(std::ifstream& file){
    size_t counter = 0;
    std::string currentStr;
    while (std::getline(file, currentStr))
   {
       ++counter;
       try
        {
           if (std::count(currentStr.begin(), currentStr.end(), '|') != 2) {
                throw graph::InvalidAdjMatrix();
            }
        }
        catch (graph::GraphException& ex)
        {
            std::cerr << ex.what();
            exit(0);
        }
    }
    file.clear();
    file.seekg(0); // ����� ��? ����������� �� 0 ���� �� ������ �����.
    return counter;
}

weight_t transformWeight(std::string& str)
{
    weight_t newWeight = std::stod(str); //
    if (newWeight < 0) return -1;
    return newWeight;
}

graph_t alg::read_graph(const char* file_name)
{
    const std::string pathToFile = file_name;
    graph_t gr;
    std::ifstream file;
    file.open(pathToFile);
    size_t dim = getNumberofNodes(file);
    for (node_name_t i = 1; i <= dim; i++)
    {
        gr.insert_node(i, node_data_t()); // ���� ��������� ���� ����������, ��������� ��������� ����������� ��� ����� ����. ��������� ��?
    }
    try
    {
        if (!file) throw graph::InvalidPath();
    }
    catch (graph::GraphException& ex)
    {
        std::cerr << ex.what();
        exit(0);
    }
    char sym;
    weight_t weight;
    size_t rows = 1;
    try {
        // �-�������� ���������� ����� vs static_cast � ��������. ��������.
        while (!file.eof())
        {
            if (!(file >> sym)) break;
            // std::cout << "sym: " << sym << "\n";
            if (sym != '|') throw graph::ExpectedAnotherSymbol();
            for (size_t cols = 1; cols <= dim; cols++)
            {
                file >> weight;
                // std::cout << "wht: " << weight << "\n";
                if (weight < 0) throw graph::AllElementsMustBePositive();
                if (weight > 0) gr.insert_edge({static_cast<node_name_t>(rows), static_cast<node_name_t>(cols)}, weight);
            }
            file >> sym;
            // std::cout << "end sym: " << sym << "\n";
            if (sym != '|') throw graph::ExpectedAnotherSymbol();
            rows++;
        }
    }
    catch (graph::GraphException& ex)
    {
        std::cerr << ex.what();
        exit(0);
    }
    catch (std::invalid_argument)
    {
        std::cerr << "Invalid argument (NotSquareMatrix)!";
        exit(0);
    }
    catch (...)
    {
        std::cerr << "Delo pahnet kerosinom.";
        exit(0);
    }
    file.close();
    return gr;

}

std::tuple<const char*, node_name_t, node_name_t> alg::parse_args(int arg_count, char* arg_vars[])
{
    // ������: dijkstra.exe --file "example.txt" --from 1 --to 6
    using namespace std::string_literals;
    try {
        if (arg_count != 7) throw graph::InvalidInputArgs();

        // lambda-������� ��� equal/hash. 
        auto equal = [](const char* left, const char* right) { return strcmp(left, right) == 0; };
        auto hash = [](const char* str) { return strlen(str); };
        std::unordered_map<const char*, const char*, decltype(hash), decltype(equal)> arg_map(3, hash, equal);

        arg_map = { {"--file", nullptr}, {"--from", nullptr}, {"--to", nullptr} };

        for (size_t i = 1; i < 7; i += 2) {
            std::string str = arg_vars[i];
            auto it = arg_map.find(arg_vars[i]);
            // ���� �� ����� �� ������ �� ������ (--file, --from, --to), �� ������.
            if (it == arg_map.end()) throw std::runtime_error("Invalid argument: "s + arg_vars[i]);
            // ���� ������ ������� ����� �� nullptr ������, ������ �� ��� ��� ������������������, ������ �������� ��� �������� �� ��������
            if (it->second != nullptr) throw std::runtime_error("Dublicated argument: "s + arg_vars[i]);
            it->second = arg_vars[i + 1]; // ����� � mapped ��������� ������� �� �������, �.�. ���� �������� ���������. 
        }
        return { arg_map["--file"], std::stoi(arg_map["--from"]), std::stoi(arg_map["--to"]) };
    }
    catch (graph::GraphException& ex)
    {
        std::cerr << ex.what();
        exit(0);
    }
    catch (std::exception& ex) {
        std::cerr << ex.what();
        exit(0);
    }
}

void alg::visualize(const char* filePath, route_t& route) {
    // std::string my_str = ""; // ���������, �������� ��������� ������� ���� � �� � ��������� �� �����. 
    // ������������ ������ ��� ������ start==finish. - ������ ������ ����. 
    std::stringstream ss;
    for (auto it = route.begin(); it != route.end(); it++) {
        if (it != route.begin()) {
            ss << " ";
        }
        ss << *it;
    }
    std::string my_str = ss.str();
    char ch = filePath[strlen(filePath) - 5]; // {num}.txt
    std::string s(1, ch);
    std::string command = "python c:/Users/User/source/repos/DijkstraImpl/Release/nxgraphdraw.py " + s + " " + my_str;
    // std::cout << command << "\n";
    system(command.c_str());

};



