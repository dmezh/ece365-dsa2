#include <chrono>
#include <iostream>
#include <sstream>

#include "graph.h"
#include "util.h"

void run_with_retry(Graph& g)
{
    clock_t c1, c2;

    for (;;) {
        std::string s;
        std::cout << "Enter name of starting vertex: ";
        std::cin >> s;

        c1 = std::clock();
        bool r = g.run_dijkstra(s);
        c2 = std::clock();

        if (r) break;

        std::cerr << "Error: vertex " << s << " does not exist!\n";
    }

    double d = (double)(c2 - c1) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << std::to_string(d) << "\n";
}

void apply_input(std::istream& input, Graph& g)
{
    std::string line;
    while (std::getline(input, line)) {
        std::string src, dst;
        Graph::Distance d;

        std::stringstream col(line);
        col >> src >> dst >> d;

        g.insert_edge(src, dst, d);
    }
}

int main()
{
    Graph g;

    auto input = open_file<std::ifstream>("graph");
    apply_input(input, g);

    run_with_retry(g);

    auto output = open_file<std::ofstream>("output");
    g.dump_distances(output);
}
