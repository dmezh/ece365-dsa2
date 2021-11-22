#include <iostream>
#include <memory>
#include <chrono>
#include <sstream>

#include "graph.h"
#include "util.h"

int main() {
    std::unique_ptr<Graph> g = std::make_unique<Graph>();

    auto input = open_file<std::ifstream>("graph");

    std::string s;
    std::cout << "Enter name of starting vertex: ";
    std::cin >> s;

    std::string line;
    while (std::getline(input, line)) {
        std::string src, dst;
        Graph::Distance d;

        std::stringstream col(line);

        col >> src >> dst >> d;
        g->insert_edge(src, dst, d);
    }

    auto c1 = std::clock();
    g->run_dijkstra(s);
    auto c2 = std::clock();

    std::cerr << "\n";
    g->dump_vertices_from_order();
    std::cerr << "\n";

    g->dump_distances();
    double d = (double)(c2 - c1) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << std::to_string(d) << "\n";
}

/*
 * 
 * v1->v21: 1
 * v21->v24: 7
 * v24->v30: 11
 * == 19
 * 
 * v1->v11: 11
 * v11->v24: 1
 * v24->v30: 11
 * == 23 ?????
 * 
 * 
 * 
 * 
 * v1->v11: 11
 * v11->v20: 4
 * v20->v28: 4
 * 
 * v1->v11: 11
 * v11->v22: 31
 * v22->v20: no edge!
 */
