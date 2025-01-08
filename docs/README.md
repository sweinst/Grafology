<div align="center">
    <img 
        src="imgs/logo.png" 
        alt="Répétiteur logo"
        height="150px"
        width="400px"
        />
</div>
<br/>
<hr/>

# Table of contents
## Notations
In the following documentation:
- $V$ is the set of vertices. The number of vertices is $\lVert V \rVert$
- $E$ is the set of edges. The number of edges is $\lVert E \rVert$
- $G = (V, E)$ is the graph defined by the vertices $V$ and the edges $E$.
- $E(i,j)$ is the edge from vertex $i$ to vertex $j$.

## Table of contents
1.  Graph classes
    1. [Graph implementations](graph_implementations.md)
    2. Graphs
    3. API
    4. &nbsp;&nbsp;Installation
2. Algorithms
    1.  All graphs
        1. Breath first search
        2. Depth first search
        3. [Transitive closure](algos/transitive_closure.md)
    2.  Undirected graphs
        1. [Bridges & Articulation Points](algos/bridges_and_AP.md)
        1. [Minimal spanning trees](algos/minimum_spanning_tree.md)
        2. [Shortest paths](algos/shortest_paths.md)
    3.  Directed graphs
        1. [Maximal flow](algos/maximum_flow.md)
        2. [Topological sorting](algos/topological_sorting.md)
