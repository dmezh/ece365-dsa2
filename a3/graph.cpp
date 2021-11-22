#include <cassert>
#include <iostream>

#include "graph.h"
#include "heap.h"

void Graph::print_rev(const Vertex* v)
{
    if (!v)
        return;

    print_rev(v->prev);

    std::cout << v->id << ", ";
}

void Graph::dump_distances()
{
    for (auto&& v : vertices_in_order) {
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

void Graph::run_dijkstra(std::string_view start)
{
    heap vertices_by_distance = heap(vertices_in_order.size() * 2);

    auto cur = get_vertex_from_lookup(start);
    if (!cur) {
        std::cerr << "Error: start vertex " << start << " does not exist!\n";
        exit(-1);
    }

    cur->distance = 0;
    cur->prev = nullptr;

    vertices_by_distance.insert(cur->id, 0, cur);

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

void Graph::Vertex::Edge::print_edge() const
{
    std::cerr << "\t[" << v->id << ", " << d << "]\n";
}

void Graph::dump_vertices_from_order()
{
    for (auto&& v : vertices_in_order) {
        std::cerr << "ID: " << v->id << "\n";
        std::cerr << "\tEdges:\n";
        for (auto const & e : v->edges) {
           e.print_edge();          
        }
    }
}

void Graph::insert_edge(std::string_view src_id, std::string_view dst_id, Distance d)
{
    // first, let's check if we already have these vertices.
    auto src = get_vertex_from_lookup(src_id);
    auto dst = get_vertex_from_lookup(dst_id);

    if (!src) {
        std::cerr << "Vertex " << src_id << " not found, adding." << std::endl;
        src = add_vertex(src_id);
    }

    if (!dst) {
        std::cerr << "Vertex " << dst_id << " not found, adding." << std::endl;
        dst = add_vertex(dst_id);
    }

    Vertex::Edge e(dst, d);
    src->edges.push_back(e);
}

Graph::Vertex* Graph::get_vertex_from_lookup(std::string_view id)
{
    bool exists;
    void *v;

    std::tie(exists, v) = vertices_lookup.getPointer(id);

    if (exists)
        return static_cast<Vertex*>(v);

    return nullptr;
}

Graph::Vertex* Graph::add_vertex(std::string_view id)
{
    auto *n = new Vertex(id);
    
    int r = vertices_lookup.insert(id, static_cast<void*>(n));
    if (r) {
        std::cerr << "Vertex " << id << ": " << r << std::endl;
        assert(0);
    }

    vertices_in_order.emplace_back(n);

    std::cerr << "Inserted vertex: " << id << std::endl;
    return n;
}
