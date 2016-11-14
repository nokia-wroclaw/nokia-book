#pragma once

#include <vector>
#include <set>
#include <iterator>

template <typename T>
class BaseGraph {
public:
    typedef unsigned int V_t;
    typedef std::set<V_t> VertexSet;

    V_t addVertex(const T &t) {
        V_t i = size++;
        edgesIn.emplace_back();
        edgesOut.emplace_back();
        data.emplace_back(t);
        enabled.emplace_back(true);
        return i;
    }

    const VertexSet &getInNeighbours(V_t x) const {
        return edgesIn[x];
    }

    const VertexSet &getOutNeighbours(V_t x) const {
        return edgesOut[x];
    }

    const T &getVertexData(V_t v) const {
        return data[v];
    }

    T &vertexData(V_t v) {
        return data[v];
    }

    bool hasEdgeFromTo(V_t a, V_t b) const {
        return edgesOut[a].find(b) != edgesOut[a].end();
    }

    std::size_t getSize() const {
        return size;
    }

    std::size_t getMaxIndex() const {
        return data.size();
    }

    std::set<V_t> vertices() const {
        std::set<V_t> xs;
        for (std::size_t i = 0; i < enabled.size(); ++i) {
            if (enabled[i]) xs.insert(i);
        }
        return xs;
    }

    void removeVertex(V_t v) {
        enabled[v] = false;
        size--;
        data[v] = T();
        for (auto u : edgesIn[v]) {
            edgesOut[u].erase(v);
        }

        for (auto u : edgesOut[v]) {
            edgesIn[u].erase(v);
        }

        edgesOut[v].clear();
        edgesIn[v].clear();
    }

    // TODO: iterators

    struct iterator: std::iterator<std::random_access_iterator_tag, V_t>
    {

        iterator() {
        }

        iterator(const std::vector<bool> &tab, size_t index = 0)
            : enabled(&tab)
            , index(index)
        {
            if (index < enabled->size() and not enabled->at(index)) incrementIndex();
        }

        iterator& operator++(int) {
            iterator i = *this;
            incrementIndex();
            return i;
        }

        iterator& operator++() {
            incrementIndex();
        }


        V_t operator*() {
            return index;
        }

        V_t operator*() const {
            return index;
        }

        void incrementIndex()
        {
            do {
                index++;
            } while ( index < enabled->size() and not enabled->at(index));
        }


        bool operator==(const iterator &o) const {
            return index == o.index;
        }

        bool operator!=(const iterator &o) const {
            return index != o.index;
        }

        const std::vector<bool> *enabled = nullptr;
        V_t index = 0;
    };


    iterator begin() const {
        return iterator{enabled, 0};
    }

    iterator end() const {
        return iterator(enabled, getMaxIndex());
    }

protected:
    std::vector<VertexSet> edgesIn;
    std::vector<VertexSet> edgesOut;
    std::vector<T> data;
    std::vector<bool> enabled;
    std::size_t size = 0;

    void removeEdgeFromTo(V_t a, V_t b) {
        edgesOut[a].erase(b);
        edgesIn[b].erase(a);
    }

    void addEdgeFromTo(V_t a, V_t b) {
        edgesOut[a].insert(b);
        edgesIn[b].insert(a);
    }

    void complement(const BaseGraph<T> &bg) {
        data = bg.data;
        enabled = bg.enabled;
        size = bg.size;

        edgesIn.resize(bg.data.size());
        edgesOut.resize(bg.data.size());

        for (auto v : bg)
        for (auto u : bg) {
            if (not bg.hasEdgeFromTo(v, u) and u != v) {
                addEdgeFromTo(v, u);
            }
        }
    }
};


template <typename T>
class UndirectedGraph: public BaseGraph<T>
{
public:
    using V_t = typename BaseGraph<T>::V_t;
    using VertexSet = typename BaseGraph<T>::VertexSet;

    void addEdge(V_t a, V_t b) {
        BaseGraph<T>::addEdgeFromTo(a, b);
        BaseGraph<T>::addEdgeFromTo(b, a);
    }

    bool hasEdge(V_t a, V_t b) const {
        BaseGraph<T>::hasEdgeFromTo(a, b);
    }

    const VertexSet &getNeighbours(V_t x) const {
        return BaseGraph<T>::getOutNeighbours(x);
    }

    void removeEdge(V_t a, V_t b) {
        removeEdgeFromTo(a, b);
        removeEdgeFromTo(b, a);
    }

    UndirectedGraph<T> complement() const {
        UndirectedGraph<T> ug;
        ug.BaseGraph<T>::complement(*this);
        return ug;
    }

    std::size_t getVertexDegree(V_t v) {
        return getNeighbours(v).size();
    }

};


template <typename T>
class DirectedGraph: public BaseGraph<T>
{
public:
    using V_t = typename BaseGraph<T>::V_t;
    using VertexSet = typename BaseGraph<T>::VertexSet;
    using BaseGraph<T>::removeEdgeFromTo;
    using BaseGraph<T>::addEdgeFromTo;
};

template <typename T>
struct DirectedGraphDirection
{
    typedef typename DirectedGraph<T>::VertexSet VertexSet;
    typedef typename DirectedGraph<T>::V_t V_t;

    virtual const VertexSet &getSuccessors(const DirectedGraph<T> &graph, V_t v) const = 0;
    virtual const VertexSet &getPredecessors(const DirectedGraph<T> &graph, V_t v) const = 0;
};

template <typename T>
struct DirectedGraphForwardDirection: public DirectedGraphDirection<T>
{
    using V_t = typename DirectedGraph<T>::V_t;
    using VertexSet = typename DirectedGraph<T>::VertexSet;

    virtual const VertexSet &getSuccessors(const DirectedGraph<T> &graph, V_t v) const {
        return graph.getOutNeighbours(v);
    }

    virtual const VertexSet &getPredecessors(const DirectedGraph<T> &graph, V_t v) const {
        return graph.getInNeighbours(v);
    }
};

template <typename T>
struct DirectedGraphBackwardDirection: public DirectedGraphDirection<T>
{
    using V_t = typename DirectedGraph<T>::V_t;
    using VertexSet = typename DirectedGraph<T>::VertexSet;

    virtual const VertexSet &getSuccessors(const DirectedGraph<T> &graph, V_t v) const {
        return graph.getInNeighbours(v);
    }

    virtual const VertexSet &getPredecessors(const DirectedGraph<T> &graph, V_t v) const {
        return graph.getOutNeighbours(v);
    }
};
