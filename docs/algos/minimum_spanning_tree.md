<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Minimum spanning tree
For an undirected graph $G(V,R)$, a *spanning tree* is a subset of edges such as:
- it touches all the vertices of the garph
- it forms a tree (G is connected without cycles)

A minimum *spanning tree (MST)* or *minimum weight spanning tree* is the spanning tree with the least total edge cost.

The code is an implementation of [Kruskal's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm)

```mermaid
---
title: Minimum spanning tree
config:
  themeVariables:
    fontSize: 10px
    sectionFontSize: 10px
    edgeLabelBackground: transparent
---
graph LR;
    A(A)
    B(B)
    C(C)
    D(D)
    E(E)
    F(F)
    G(G)
    A -- 7 --- B
    A -- 5 --- D
    B -- 8 --- C
    B -- 9 --- D
    B -- 7 --- E
    C -- 5 --- E
    D -- 15 --- E
    D -- 6 --- F
    F -- 8 --- E
    F -- 11 --- G
    E -- 9 --- G

    linkStyle default font-size: 15px
    linkStyle 0,1,4,5,7,10 stroke-width:2px, stroke:red, fill: none
```


## Requirements
- The graph must be undirected
- The graph must be acyclic
- The graph can't have disjoint sub-graphs

## Complexity
- The complexity is $O(\lVert E \rVert \log{(\lVert E \rVert)})$

## Usage
```C++
    #include <grafology/algorithms/minimum_spanning_tree.h>
    namespace g = grafology;

    g::SparseUndirectedGraph graph(20);
    // ....
    auto mst = g::minimum_spanning_tree(graph);
    // ....

```
