/*
* This example is an implementation of the classical algorithm used for detecting arbitrage opportunities in the foreign exchange market.
* The graph is built with the currencies as vertices and -log(exchange rate) as the edge weights.
* As the algorithms cannot handle the multiplication of the rates, one use the log of the rates to convert the multiplications to additions.
* An arbitrage is found, if the multiplication of the rates on a cycle is greater than 1.
* So, if -log(rate1) -log(rate2) -log(rate3) > 0. 
* So, if one can detect a negative cycle (Bellman-Ford algorithm), then there is an arbitrage opportunity.
*/

#include "fx_rates.h"
#include <grafology/graph.h>
#include <cmath>

namespace g = grafology;
using weight_t = double;
using edge_t = g::edge_t<weight_t>;
using vertex_t = g::vertex_t;
using Graph = g::DenseGraphImpl<weight_t>;

int main() {
    auto n_ccys = currencies.size();
    // build the graph
    Graph graph(n_ccys, n_ccys, true);
    for (size_t i = 0; i < n_ccys; ++i) {
        for (size_t j = 0; j < n_ccys; ++j) {
            if (i != j && rates[i][j] != 0) {
                graph.set_edge(i, j, -std::log(rates[i][j]));
            }
        }
    }
    return 0;
}
