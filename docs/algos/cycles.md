<div align="center">
    <img 
        src="../imgs/logo.png" 
        alt="Grafology logo"
        height="113px"
        width="300px"
        />
</div>
<hr/>

# Cycles

## Negative cycles
A *negative-weight* cycle in a directed graph is a cycle whose total weight is negative. 

It is not practical to test all possible paths as their number can grow exponentially with the graph size.

The implementation use the [Bellman-For algorithm](https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm)

### Requirements
- The graph must directed

### Complexity
- O$(\lVert V \rVert \lVert E \rVert)$

## Example

An example can be found in the [FX arbitrage detection example](../../examples/fx_arbitrage.cpp)
