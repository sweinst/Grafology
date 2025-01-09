<div align="center">
    <img 
        src="imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Limitations
At the moment:
- Graphs have a fixed capacity and cannot be changed. So, at the moment a graph must be created with a capacity set to the expected maximum number of nodes.
- Vertices cannot be removed
- There can't be more than one edge from one vertex to another vertex. But there can be an edge in the opposite direction.

This will be fixed in upcoming releases

# Implementations
The "implementation" classes:
 - use ```vertex_t``` (unsigned integers) as Vertex ids
 - vertex ids are set by the classes and cannot be set or modified directly.
 - use ```edge_t``` for representing vertices:
 ```C++
 struct edge_t {
    vertex_t start;
    vertex_t end;
    int weight = 1;
};
 ```

- All vertices have an integer weight (defaulted to 1). An edge can be removed by setting its weight to zero.

 There are 2 types of implementations:
 - ```SparseGraphImpl``` which uses an *adjacency list* for representing the graph
 - ```DenseGraphImpl``` which uses an *adjacency matrix* for representing the graph

Implementations can be used directly. They are lighter (all identifiers are integers and no extra information is stored), faster (no translation needed between the user code identifiers amd the graph internal ids).

So in this case, it will be up to the user to maintain vertex ids to its own ids.

## Graph Implementation types
### Sparse graphs
<img src="imgs/adjacency-list.svg"/>

Sparse graphs use an a array which size has been set to the graph *capacity*. So each vertex is allocated a cell. 

Each cell give access to an *adjacency list* which store all the edges stating at that vertex as well as their weight.

If the weight of an edge is set to zero, the edge is removed from the adjacency list.

When dealing with an **undirected graph**, each edge is *"duplicated"*. So if there is an edge from vertex $i$ to vertex $j$, there will an edge from vertex $j$ to vertex $i$.

### Dense graphs
<img src="imgs/adjacency-matrix.svg"/>

Dense graphs use a square matrix for representing edges. The matrix size is set to the graph *capacity*. So each vertex is allocated a row and a column. 

There is an edge from vertex $i$ to vertex $j$ if the value of the adjacency matrix at row $i$ and column $j$ is not zero. This value is the edge weight.

When dealing with an **undirected graph**, the adjacency matrix is symmetrical.

## Graph types

On top of the implementations, "graphs" are wrappers around the "implementation". They allow users to use their own vertex identifiers as well as storing extra data.
