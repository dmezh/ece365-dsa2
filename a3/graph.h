#ifndef GRAPH_H
#define GRAPH_H

#include <list>

#include "hash.h"
#include "heap.h"

class Graph {
    public:
        typedef int Distance;

        Graph() {};
        ~Graph();

        void insert_edge(std::string_view n1, std::string_view n2, Distance d);
        void dump_vertices_from_order();

        void run_dijkstra(std::string_view start);
        

        void dump_distances();

    private:
        class Vertex {
            public:
                class Edge {
                    public:
                        Graph::Vertex* v;
                        Distance d;

                        Edge(Graph::Vertex* v, Distance d) : v(v), d(d) {};

                        void print_edge() const;
                };
                std::list<Edge> edges;
                std::string id;

                Distance distance = -1;
                Vertex* prev = nullptr;
                bool visited = false;

                Vertex(std::string_view id) : id(id) {};
        };

        hashTable vertices_lookup = hashTable(100);
        heap vertices_by_distance = heap(100);
        std::list<Vertex*> vertices_in_order;
        
        
        Vertex* get_vertex_from_lookup(std::string_view id);
        Vertex* add_vertex(std::string_view id);

        void print_rev(Vertex* v);
};

#endif
