#include <ostream>

#include "graph.h"
#include "heap.h"

void Graph::print_rev(const Vertex* v, std::ostream& out)
{
    if (!v)
        return;

    print_rev(v->prev, out);

    out << v->id << ", ";
}

void Graph::dump_distances(std::ostream& out)
{
    for (auto&& v : vertices_in_order) {
        out << v->id << ": ";

        if (!v->distance_valid()) {
            out << "NO PATH\n";
            continue;
        }

        out << v->distance;

        out << " [";
        print_rev(v->prev, out);
        out << v->id << "]";

        out << "\n";
    }
}

// true if run finished, false if start vertex does not exist
bool Graph::run_dijkstra(std::string_view start)
{
    heap vertices_by_distance = heap(vertices_in_order.size() * 2);

    auto cur = get_vertex_from_lookup(start);
    if (!cur)
        return false;

    cur->distance = 0;
    cur->prev = nullptr;

    vertices_by_distance.insert(cur->id, 0, cur);

    while (!vertices_by_distance.deleteMin(nullptr, nullptr, &cur)) {
        if (cur->visited) continue;

        for (auto const & e : cur->edges) {
            switch (vertices_by_distance.insert(e.v->id, e.d, e.v)) {
                case 0:
                    break;
                case 1:
                    abort();
                case 2:
                    break;
            }

            Distance new_dist = cur->distance + e.d; // distance of neighbor from start

            if (!e.v->distance_valid() || new_dist < e.v->distance) { // update distance
                e.v->distance = new_dist;
                e.v->prev = cur;
                vertices_by_distance.setKey(e.v->id, new_dist);
            }
        }

        cur->visited = true;
    }

    return true;
}

void Graph::Vertex::Edge::print_edge() const
{
    // std::cerr << "\t[" << v->id << ", " << d << "]\n";
}

void Graph::dump_vertices_from_order()
{
    for (auto&& v : vertices_in_order) {
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

    if (!src)
        src = add_vertex(src_id);

    if (!dst)
        dst = add_vertex(dst_id);

    src->edges.emplace_back(dst, d);
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
    
    // vertices_lookup takes ownership
    vertices_lookup.insert(id, static_cast<void*>(n));

    vertices_in_order.emplace_back(n);

    return n;
}
