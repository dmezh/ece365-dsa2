#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <ostream>
#include <memory>

#include "hash.h"

class Graph {
    public:
        typedef int Distance;

        /*
         * Insert the given edge (specified by the source and destination vertex ID and distance)
         * into the graph.
         *
         * The vertices will be created and added to the graph if necessary.
         */
        void insert_edge(std::string_view src_id, std::string_view dst_id, Distance d);

        /*
         * Run Dijkstra's algorithm on the graph, starting from the given vertex ID.
         *
         * returns true if run was completed
         * returns false if given start vertex ID was not in the graph.
         */
        bool run_dijkstra(std::string_view start);

        /*
         * Dump the paths and distances for the vertices in the graph, in the order they were
         * originally encountered.
         *
         * Format for each vertex, one per line:
         *
         * (vertex ID): (path length) [(start vertex ID), (next vertex ID), ... (vertex ID)]
         *
         * Example:
         *
         * v1: 0 [v1]
         * v2: 2 [v1, v2]
         */
        void dump_distances(std::ostream& out);

    private:
        class Vertex {
            public:
                class Edge {
                    public:
                        Graph::Vertex *v;
                        Distance d;

                        Edge(Graph::Vertex *v, Distance d) : v(v), d(d){};
                };

                std::list<Edge> edges;
                std::string id;

                Distance distance = -1;
                Vertex *prev = nullptr;

                bool visited = false;

                Vertex(std::string_view id) : id(id){};

                bool distance_valid() { return (distance != -1); }
        };

        hashTable vertices_lookup = hashTable(2);
        std::list<std::unique_ptr<Vertex>> vertices_in_order;

        Vertex *get_vertex_from_lookup(std::string_view id);
        Vertex *add_vertex(std::string_view id);

        void print_rev(const Vertex *v, std::ostream& out);
};

#endif
