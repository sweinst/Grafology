<div align="center">
	<img 
		src="docs/imgs/logo.png" 
		alt="Répétiteur logo"
		width="600em"
		/>
</div>
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



