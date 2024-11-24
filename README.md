<div align="center">
	<img 
		src="docs/imgs/logo.png" 
		alt="Répétiteur logo"
		width="600em"
		/>
</div>
<br/>
<br/>
<hr/>

# A simple graph library
*Graphology* is yet another graph library implementing various classical algorithms. It uses various C++23/20 features like: coroutines/generators, concepts, ranges/views, ... in order to generate an easy API.

## Graph types
There are two types of graph classes:
- Classes implementing the GraphImpl concept
- Specializations of the template class Graph

GraphImpl are the core classes:
- vertices are identified with integers. 
- Edges are identified by the starting and ending
- All edges have an integer weight
- There can be only one "directed" edge between two vertices. But there can be 2 edges if they are of opposite direction.

Graph classes are wrappers around the GraphImpl classes and allow to define vertices with other types than integers.

Both types can be used. Graph instances add an extra layer which can impact performances (when dealing with a large number of vertices) but can ease their use.

## Graph implementations
There are two type of implementations:
- Sparse graphs which use an adjacency list for the edges
- Dense graphs which uses an adjacency matrix for the edges

## Directed and undirected graphs.
Graph and GraphImpl can be:
- Directed
- Undirected. In this case, there will be always 2 "directed" node between 2 vertices with the same weight buf with opposite direction

## Example
```mermaid
%%{init: {'themeVariables': {'fontSize': 9,'sectionFontSize': 9}}}%%
graph LR;
A(Mix ingredients) --> B(Add batter to pan) 
C(Grease/Flour pan) --> B(Add batter to pan) 
B(Add batter to pan) --> D(Bake cake) 
E(Preheat oven) --> D(Bake cake) 
D(Bake cake) --> F(Cool cake) 
F(Cool cake) --> G(Frost cake) 
H(Make frosting) --> G(Frost cake) 
G(Frost cake) --> i(Eat cake 😀) 
```


```C++
#include <grafology/grafology.h>
#include <print>

namespace g = grafology;
// a directed sparse graph which uses strings as vertices identifiers
using Graph = g::DirectedSparseGraph<std::string>;
using Edge = Graph::Edge;

std::vector<Edge> edges {
        {"Mix ingredients", "Add batter to pan"}, 
        {"Grease/Flour pan", "Add batter to pan"}, 
        {"Add batter to pan", "Bake cake"}, 
        {"Preheat oven", "Bake cake"}, 
        {"Bake cake", "Cool cake"}, 
        {"Cool cake", "Frost cake"}, 
        {"Make frosting", "Frost cake"}, 
        {"Frost cake", "Eat cake :-)"}, 
    };


int main () {
    Graph graph(10);
    graph.set_edges(edges, true);
    for (const auto& [group, vertex] : g::topological_sort(graph)) {
        std::println( "Group {}: '{}'", group, vertex);
    }
    return 0;
}
```
Output:
```bash
Group 0: 'Mix ingredients'
Group 0: 'Grease/Flour pan'
Group 0: 'Preheat oven'
Group 0: 'Make frosting'
Group 1: 'Add batter to pan'
Group 2: 'Bake cake'
Group 3: 'Cool cake'
Group 4: 'Frost cake'
Group 5: 'Eat cake :-)'
```