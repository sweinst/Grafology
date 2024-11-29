<div align="center">
    <img 
        src="imgs/logo.png" 
        alt="Répétiteur logo"
        height="150px"
        width="400px"
        />
</div>
<br/>

# Limitations
At the moment:
- Graphs have a fixed capacity and cannot be resized
- Vertices cannot be removed

This will be fixed in upcoming releases

# Implementations
The "implementation" classes:
 - use ```vertex_t``` (unsigned integers) as Vertex ids
 - vertex ids are set by the classes and cannot be set or modified 
 - use ```edge_t``` for representing vertices:
 ```C++
 struct edge_t {
    vertex_t start;
    vertex_t end;
    int weight = 1;
};
 ```

 There are 2 types of implementations:
 - ```SparseGraphImpl``` which uses an *adjacency list* for representing the graph
 - ```DenseGraphImpl``` which uses an *adjacency matrix* for representing the graph
 
Implementations can be directly used but it will be up to the user to maintain vertex ids to its own ids.

## Creation


# Sparse graphs
<img src="imgs/adjacency-list.svg"/>

# Dense graphs
<img src="imgs/adjacency-matrix.svg"/>
