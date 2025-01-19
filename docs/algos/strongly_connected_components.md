<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Strongly Connected Components
Given a directed grap $G(V, E)$, a **Strongly Connected Component** is a subset of $G$, such as:
- any two vertices in the *SCC*  are mutually reachable. I.e., for any vertices $u$ and $v$ of the *SCC*, there is a directed path between $u$ and $v$.
- the addition of any vertex to the ^SCC* breaks the first condition

The code is an implementation of [Tarjan's strongly connected components algorithm](https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm)

```mermaid
---
title: Strongly Connected Components
config:
  themeVariables:
    fontSize: 10px
    sectionFontSize: 10px
    edgeLabelBackground: transparent
---
graph LR;
    1(1):::SCC1
    2(2):::SCC1
    3(3):::SCC1
    4(4):::SCC3
    5(5):::SCC4
    6(6):::SCC2
    7(7):::SCC2
    8(8):::SCC2

    1 --- 2
    2 --- 3
    2 --- 4
    2 --- 5
    3 --- 1
    3 --- 4
    5 --- 6
    5 --- 7
    6 --- 8
    7 --- 6
    8 --- 7

    classDef SCC1 stroke:teal,stroke-width:3px
    classDef SCC2 stroke:DeepPink,stroke-width:3px
    classDef SCC3 stroke:DodgerBlue,stroke-width:3px
    classDef SCC4 stroke:Chocolate,stroke-width:3px
    linkStyle default font-size: 15px
```

## Requirements
- The graph must be directed

## Complexity
- The performance is $O(\lVert V \rVert + \lVert E \rVert)$.

## Usage
```C++
    #include <grafology/algorithms/strongly_connected_components.h>
    namespace g = grafology;

    g::SparseDirectedGraph graph(20);
    // ....
    for (const auto& scc: g:strongly_connected_components(graph)) {
        // scc is a vector containing the vertices defining the SCC
        // ...
    }
    // ....

```
