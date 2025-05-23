/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>     // assert
#include <limits>      // std::numeric_limits
#include <algorithm>   // std::make_heap(), std::pop_heap(), std::push_heap()

#include "graph.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(n + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge e
// update weight if edge e is present
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.from])) {
            T[e1.from].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());
}

// remove undirected edge e
void Graph::removeEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edgeRemoval = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                               [e1](const Edge &ed) { return e1.links_same_nodes(ed); });

        assert(it != end(T[e1.from]));
        T[e1.from].erase(it);  // remove edge e1
        --n;                   // decrement the counter of edges
    };

    edgeRemoval(e);
    edgeRemoval(e.reverse());
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    int v = 1;
    dist[v] = 0; // start can be any vertex
    done[v] = true;

    int total_weight = 0;

    while (true) {
        for (auto& edge : table[v]) {
            int u = edge.to;
            if (done[u] == false && dist[u] > edge.weight) {
                path[u] = v;
                dist[u] = edge.weight;
            }
        }
        int min_dist_val = std::numeric_limits<int>::max();
        // since vertices are 1-indexed.

    // Iterate through all potential vertices (1 to size)
        for (int i = 1; i <= size; ++i) {
            if (!done[i] && dist[i] < min_dist_val) {
                min_dist_val = dist[i];
                v = i;
            }
        }
        if (min_dist_val == std::numeric_limits<int>::max()) { break; }
        done[v] = true;
        if (path[v] != 0) { total_weight += min_dist_val; }
        std::cout << Edge(path[v], v, dist[v]) << std::endl;
    }

    std::cout << "Total weight = " << total_weight << std::endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***
    std::vector<Edge> heap;          // Heap

    for (int i = 1; i <= size; ++i) {
        for (const auto& e : table[i]) {
            if (e.from < e.to) {
                heap.push_back(e);
            }
        }
    }

    std::make_heap(heap.begin(), heap.end(), std::greater<Edge>());   // Make min heap -> cpp reference
    DSets D{ size };                            // Dset

    int totalWeight = 0;
    int edgeCount = 0;

    while (edgeCount < size - 1 && !heap.empty()) {
        std::pop_heap(heap.begin(), heap.end(), std::greater<Edge>{});
        Edge minEdge = heap.back();
        heap.pop_back();

        int uSet = D.find(minEdge.from);
        int vSet = D.find(minEdge.to);

        if (uSet != vSet) {
            D.join(uSet, vSet);
            std::cout << minEdge << std::endl;
            totalWeight += minEdge.weight;
            ++edgeCount;
        }
    }
    std::cout << "\nTotal weight = " << totalWeight << std::endl;
}

// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
