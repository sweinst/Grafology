<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Bridges and Articulation Points
## Bridges
A *bridge* of a graph $G(E,V)$ is an edge such as if the edge is removed, then number of disconnected components is increased.

The code is an implementation of [Tarjan's bridge-finding algorithm](https://www.geeksforgeeks.org/bridge-in-a-graph)

```mermaid
---
title: Bridges
config:
  themeVariables:
    fontSize: 10px
    sectionFontSize: 10px
    edgeLabelBackground: transparent
---
graph LR;
    0(0)
    1(1)
    2(2)
    3(3)
    4(4)
    5(5)
    6(6)
    7(7)
    8(8)
    9(9)
    10(10)
    11(11)
    12(12)

    0 --- 1
    0 --- 3
    1 --- 2
    1 --- 4
    3 --- 2
    3 --- 7
    4 --- 5
    5 --- 6
    6 --- 4
    7 --- 8
    9 --- 10
    10 --- 11
    11 --- 9
    11 --- 12

    linkStyle default font-size: 15px
    linkStyle 3,5,9,13 stroke-width:2px, stroke:red, fill: none
```

*NB: a bug in Mermaid shows "undirected" edges as "directed". So all the arrows should not been drawn.*

## Requirements
- The graph must be undirected

## Complexity
- The performance is $O(\lVert V \rVert + \lVert E \rVert)$.

## Usage
```C++
#include <grafology/algorithms/bridges.h>
namespace g = grafology;

g::SparseDirectedGraph graph(20);
// ....
for (const auto& edge: g::bridges(graph) {
    // ....
}

```
