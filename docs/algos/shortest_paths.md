<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Répétiteur logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Shortest paths

## Shortest path from one vertex to another vertex
This algorithm allows to find the path from one vertex to another. The path distance is the sum of all the edge weights along the path.

The implementation is based on the [A<sup>*</sup> algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm#)

It requires a *cost function* which allows to estimate the distance from a vertex to the destination vertex

### Requirements
- The graph must be undirected
- All weights must be positive

### Usage
The algorithm returns a *ShortestPaths* instance which allows to:
- check if the destination vertex is reachable (*is_reachable*)
- get the distance to the destination vertex (*get_distance*). If the vertex is unreachable, it will return *graphology::D_INFINITY*
- get an iterator for the path leading to the destination vertex (*get_path*)

```C++
#include <grafology/shortest_path.h>
namespace g = grafology;


g::SparseUndirectedGraph graph(20);
Vertex start;
Vertex end;

double computeDistance(const Vertex& v1, const Vertex& v2) {
    return // ....
}

// ....

auto result = g::shortest_path(g, start, end, computeDistance));
for (const auto& [vertex, weight] : result.get_path()) {
    // ....
}

```

### Complexity
- The work case performance is  $O(\lVert E \rVert \log{(\lVert V \rVert)})$

## Shortest paths from one vertex to all other vertices
This algorithm find the shortest paths from one vertex to all the other vertices. The path distance is the sum of all the edge weights along the path.

The implementation is based on the [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).

### Requirements
- The graph must be undirected
- All weights must be positive

### Complexity
- The work case performance is  $O(\lVert E \rVert + \lVert V \rVert \log{(\lVert V \rVert)})$

### Usage
The algorithm returns a *AllShortestPaths* instance which allows to:
- check if another vertex is reachable (*is_reachable*)
- get the distance to another vertex (*get_distance*). If the vertex is unreachable, it will return *graphology::D_INFINITY*
- get an iterator for the path leading to another vertex (*get_path*)


```C++
#include <grafology/all_shortest_paths.h>
namespace g = grafology;

g::SparseUndirectedGraph graph(20);
Vertex start;
Vertex another_vertex;

// ....

auto result = g::all_shortest_paths(g, start));
for (const auto& [vertex, weight] : result.get_path(another_vertex)) {
    // ....
}
```
