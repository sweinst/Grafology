<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Minimum spanning tree
For an undirected graph $G(V,E)$, a *spanning tree* is a subset of edges such as:
- it touches all the vertices of the graph
- it forms a tree (G is connected without cycles)

A minimum *spanning tree (MST)* or *minimum weight spanning tree* is the spanning tree with the least total edge cost.

The code is an implementation of [Kruskal's algorithm](https://en.wikipedia.org/wiki/Kruskal%27s_algorithm)

```mermaid
---
title: Input graph
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
    H(H)
    I(I)
    A -- 4 --- B
    A -- 8 --- G
    B -- 8 --- C
    B -- 11 --- G
    C -- 7 --- D
    C -- 2 --- I
    C -- 4 --- F
    D -- 9 --- E
    D -- 14 --- F
    E -- 10 --- F
    F -- 2 --- H
    G -- 1 --- H
    G -- 7 --- I
    I -- 6 --- H

    linkStyle default font-size: 15px
    linkStyle 0,2,4,5,6,7,10,11 stroke-width:3px, stroke:red, fill: none
```


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
    H(H)
    I(I)
    A -- 4 --- B
    B -- 8 --- C
    C -- 7 --- D
    C -- 2 --- I
    C -- 4 --- F
    D -- 9 --- E
    F -- 2 --- H
    H -- 1 --- G

    linkStyle default font-size: 15px, fill: none
```

*NB: a bug in Mermaid shows "undirected" edges as "directed". So all the arrows should not been drawn.*

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
