/*********************************************
 * file:	~\code4a\digraph.cpp              *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <limits>  //std::numeric_limits
#include <vector>
#include <cassert>
#include <queue>
#include <format>

#include "digraph.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n)
    : table(n + 1)  // slot zero not used
    , size{n}       // number of verices
    , n_edges{0}
    , dist(n + 1)
    , path(n + 1)
    , done(n + 1) {
    assert(n >= 1);
    // Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert directed edge e = (u, v, w)
// update weight w if edge (u, v) is present
void Digraph::insertEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    // Check if edge e already exists
    if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                               [e](const Edge& ed) { return e.links_same_nodes(ed); });
        it == end(table[e.from])) {
        table[e.from].push_back(e);  // insert new edge e
        ++n_edges;
    } else {
        it->weight = e.weight;  // update the weight
    }
}

// remove directed edge e
void Digraph::removeEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                           [e](const Edge& ed) { return e.links_same_nodes(ed); });

    assert(it != end(table[e.from]));
    table[e.from].erase(it);
    --n_edges;
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
    assert(s >= 1 && s <= size);

    std::queue<int> Q;

    for (int v = 1; v <= size; ++v) {
        dist[v] = std::numeric_limits<int>::max();
        path[v] = 0;
    }
    dist[s] = 0;
    Q.push(s);

    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();

        //  iterates through all outgoing edges from vertex
        // table[v] is the adjacency list for vertex v
        for (auto& edge : table[v]) {
            // edge.to gets the destination vertex u of the current edge
            if (dist[edge.to] == std::numeric_limits<int>::max()) {
                dist[edge.to] = dist[v] + 1;    // Distance to u is distance to v + 1 (unweighted edge)
                path[edge.to] = v;              // Set v as the previous of u in the shortest path
                Q.push(edge.to);                // Add u to the queue to visit its neighbors
            }
        }
    }
}

// construct positive weighted single source shortest path-tree for start vertex s
// Dijktra’s algorithm
void Digraph::pwsssp(int s) const {
    assert(s >= 1 && s <= size);

    for (int v = 0; v <= size; ++v) {
        dist[v] = std::numeric_limits<int>::max();
        path[v] = 0;
        done[v] = false;
    }
    
    dist[s] = 0;
    int v = s;
    done[s] = true;
    while (true)
    {
        for (auto& edge : table[v]) {
            if (done[edge.to] == false && dist[edge.to] > (dist[v] + edge.weight))
            {
                dist[edge.to] = dist[v] + edge.weight;
                path[edge.to] = v;
            }
        }
        v = find_smallest_undone_distance_vertex();
        if (v == 0) break; // exit the loop
        done[v] = true;
            
    }

}

int Digraph::find_smallest_undone_distance_vertex() const {
    int min_dist_val = std::numeric_limits<int>::max();
    int best_v_idx = 0; // Using 0 to indicate "no suitable vertex found"
    // since vertices are 1-indexed.

// Iterate through all potential vertices (1 to size)
    for (int v = 1; v <= size; ++v) {
        if (!done[v] && dist[v] < min_dist_val) {
            min_dist_val = dist[v];
            best_v_idx = v;
        }
    }
    return best_v_idx;
}

// print graph
void Digraph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : ", v);
        for (auto const& e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
    std::cout << "vertex    dist    path\n";
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : {:6} {:6}\n", v,
                                 ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
                                 path[v]);
    }
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
    assert(t >= 1 && t <= size);
    printPathRecursion(t);
    std::cout << " (" << dist[t] << ")\n";
}

void Digraph::printPathRecursion(int t) const {
    if (path[t] == 0) {
        std::cout << " " << t;
        return;
    }

    printPathRecursion(path[t]);
    std::cout << " " << t;
}