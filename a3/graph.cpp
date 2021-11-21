#include <cassert>
#include <iostream>

#include "graph.h"

void Graph::print_rev(Vertex* v) {
    if (!v)
        return;

    print_rev(v->prev);

    std::cout << v->id << ", ";
}

void Graph::dump_distances() {
    for (auto const *v : vertices_in_order) {
        std::cout << v->id << ": ";

        if (v->distance == -1) {   
            std::cout << "NO PATH\n";
            continue;
        }

        std::cout << v->distance;

        std::cout << " [";
        print_rev(v->prev);
        std::cout << v->id << "]";

        std::cout << "\n";
    }
}

void Graph::run_dijkstra(std::string_view start) {
    // yeah buddy
    auto cur = get_vertex_from_lookup(start);

    assert(!vertices_by_distance.insert(cur->id, 0, cur));
    cur->distance = 0;
    cur->prev = nullptr;

    while (!vertices_by_distance.deleteMin(nullptr, nullptr, &cur)) {
        if (cur->visited) continue;
        std::cerr << "Visiting vertex: " << cur->id << "\n";
        for (auto const & e : cur->edges) {
            std::cerr << "visiting edge:";
            e.print_edge();

            switch (vertices_by_distance.insert(e.v->id, e.d, e.v)) {
                case 0:
                    std::cerr << "Vertex inserted! ID: " << e.v->id << "\n";
                    break;
                case 1:
                    // HEAP RESIZE NEEDED
                    abort();
                case 2:
                    std::cerr << "Vertex " << e.v->id << " found, not inserting\n";
                    break;
            }

            Distance new_dist = cur->distance + e.d; // distance of neighbor from start
            if (e.v->distance == -1 || new_dist < e.v->distance) {
                std::cerr << "Updating new shortest distance for [" << e.v->id << "]: " << new_dist << "\n";
                e.v->distance = new_dist;
                e.v->prev = cur;
                vertices_by_distance.setKey(e.v->id, new_dist);
            } else {
                std::cerr << "Not updating new shortest distance, old=" << e.v->distance << ", new=" << new_dist << "\n";
            }
        }
    cur->visited = true;
    }
}

void Graph::Vertex::Edge::print_edge() const {
    std::cerr << "\t[" << v->id << ", " << d << "]\n";
}

void Graph::dump_vertices_from_order() {
    for (auto const & v : vertices_in_order) {
        std::cerr << "ID: " << v->id << "\n";
        std::cerr << "\tEdges:\n";
        for (auto const & e : v->edges) {
           e.print_edge();          
        }
    }
}

void Graph::insert_edge(std::string_view src_id, std::string_view dst_id, Distance d) {
    // first, let's check if we already have these vertices.
    auto src = get_vertex_from_lookup(src_id);
    auto dst = get_vertex_from_lookup(dst_id);

    if (!src) {
        src = add_vertex(src_id);
    }

    if (!dst) {
        dst = add_vertex(dst_id);
    }

    auto e = Vertex::Edge(dst, d);
    src->edges.push_back(e);

    // hash table will auto-resize as needed. HEAP WILL NOT!
}

Graph::Vertex* Graph::get_vertex_from_lookup(std::string_view id) {
    bool exists;
    void *v;

    std::tie(exists, v) = vertices_lookup.getPointer(id);

    if (exists)
        return static_cast<Vertex*>(v);

    return nullptr;
}

Graph::Vertex* Graph::add_vertex(std::string_view id) {
    auto *n = new Vertex(id);
    assert(!vertices_lookup.insert(id, static_cast<void*>(n)));

    vertices_in_order.push_back(n);

    return n;
}

Graph::~Graph() {
    for (auto *v : vertices_in_order) {
        delete v;
    }
}
