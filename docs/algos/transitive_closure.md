<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Transitive closure
This algorithm transforms an existing graph $G(V,E)$ by adding edges between node which are indirectly connected.

So in the resulting graph $\forall (i,j,k)$, if the edges $G(i,k)$ and $G(k,j)$ exist, the edge $G(i,j)$ will be added if it doesn't exist.

```mermaid
---
title: Initial graph
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
    A --> B
    A --> C
    C --> D
    D --> E
    E --> F
```

```mermaid
---
title: Resulting graph
config:
  themeVariables:
    fontSize: 9px
    sectionFontSize: 9px
    edgeLabelBackground: transparent
---
graph LR;
    A(A)
    B(B)
    C(C)
    D(D)
    E(E)
    F(F)
    A --> B
    A --> C
    A --> D
    A --> E
    A --> F
    C --> D
    C --> E
    C --> F
    D --> E
    D --> F
    E --> F
```


## Requirements
- No requirements

## Complexity
- For a dense graph (highly connected), the complexity is $O(\lVert V \rVert^3)$
- But it tends to $O(\lVert V \rVert^2)$, the sparser the graph is.

## Usage
```C++
    #include <grafology/algorithms/transitive_closure.h>
    namespace g = grafology;

    g::SparseDirectedGraph graph(20);
    // ....
    g::transitive_closure(graph);
    // ....

```
