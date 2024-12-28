<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Topological sorting
Topological sorting is a linear ordering of vertices such that for every directed edge $u \rightarrow v$, vertex $u$ comes before $v$ in the ordering.

Here a level index is also given with the vertices. In the example below, if vertices where tasks:
- tasks in the group 0 can be run concurrently
- tasks in the group 1 can be run concurrently once all tasks of group 0 have been completed
- tasks in the group 2 can be run concurrently once all tasks of group 1 have been completed
- ...

The code is an implementation of [Kahn's algorithm](https://en.wikipedia.org/wiki/Topological_sorting#Kahn's_algorithm)


```mermaid
---
title: Input graph
config:
  themeVariables:
    fontSize: 9px
    sectionFontSize: 9px
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
    J(J)
    A --> B
    A --> C
    B --> C
    D --> C
    D --> E
    B --> F
    D --> F
    F --> G
    E --> H
    I --> J
    G ~~~ I
```

```mermaid
---
title: Resulting flow
config:
  themeVariables:
    fontSize: 10px
    sectionFontSize: 10px
---
graph LR;
    Group0(A
    D
    I)
    Group1(B
    E
    J)
    Group2(C
    F
    H)
    Group3(G)
    Start(( ))
    
    Start -- Group 0 --> Group0
    Group0 -- Group 1  --> Group1
    Group1 -- Group 2 --> Group2
    Group2 -- Group 3 --> Group3
```

## Requirements
- The graph must be directed
- The graph must be acyclic

## Complexity
- The complexity is $O(\lVert V \rVert + \lVert E \rVert)$

## Usage
```C++
    #include <grafology/algorithms/topological_sort.h>
    namespace g = grafology;

    g::SparseDirectedGraph graph(20);
    // ....
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
        // ....
    }

```
