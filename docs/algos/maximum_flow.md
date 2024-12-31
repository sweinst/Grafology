<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Maximum flow
Given a directed graph $G(V,E)$ which represents a flow network where edge weights are the edge capacities.
We want to find the maximum possible flow between the source $s$ and the sink $e$.

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
    0(0)
    1(1)
    2(2)
    3(3)
    4(4)
    5(5)
    0 -- 16 --> 1
    0 -- 13 --> 2
    1 -- 10 --> 2
    1 -- 12 --> 3
    2 -- 4 --> 1
    2 -- 14 --> 4
    3 -- 9 --> 2
    3 -- 20 --> 5
    4 -- 7 --> 3
    4 -- 4 --> 5

    linkStyle default font-size: 15px
```

```mermaid
---
title: Maximum flow 23
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
    0 -- 11 --> 1
    0 -- 12 --> 2
    1 --> 2
    1 -- 12 --> 3
    2 -- 1 --> 1
    2 -- 11 --> 4
    3 --> 2
    3 -- 19 --> 5
    4 -- 7 --> 3
    4 -- 4 --> 5

    linkStyle default font-size: 15px
    linkStyle 0,1,3,4,5,7,8,9 stroke-width:2px, stroke:red, fill: none
```

## Requirements
- The graph must be directed

## Complexity
- The work case performance is $O(\lVert E \rVert f)$ where $f$ is the maximum capacity.

## Usage
```C++
#include <grafology/algorithms/maximum_flow.h>
namespace g = grafology;

g::SparseDirectedGraph graph(20);
Vertex start;
Vertex end;
// ....
auto max_flow = g::maximum_flow(graph, start, end);
// ....

```

## Notes
A possible better algorithm is the [Push Relabel Algorithm](https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm). This will be investigated later.

