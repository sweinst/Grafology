/*
* This example is an implementation of the classical algorithm used for detecting arbitrage opportunities in the foreign exchange market.
* The graph is built with the currencies as vertices and -log(exchange rate) as the edge weights.
* As the algorithms cannot handle the multiplication of the rates, one use the log of the rates to convert the multiplications to additions.
* An arbitrage is found, if the multiplication of the rates on a cycle is greater than 1.
* So, if -log(rate1) -log(rate2) -log(rate3) - ... > 0. 
* So, if one can detect a negative cycle (Bellman-Ford algorithm), then there is an arbitrage opportunity.
*/

#include "fx_rates.h"
#include <grafology/algorithms/cycles.h>
#include <cmath>
#include <print>
#include <set>

namespace g = grafology;
using weight_t = double;
using vertex_t = g::vertex_t;
using Graph = g::DenseGraphImpl<weight_t>;
using Cycle = std::vector<vertex_t>;
using CycleId = std::set<vertex_t>;


int main() {
    // the currencies matrix is built with the help of the fx_rates.py Python script
    // running the script will generate the fx_rates.h file
    auto n_ccys = currencies.size();
    // build the graph
    Graph graph(n_ccys, n_ccys, true);
    for (size_t i = 0; i < n_ccys; ++i) {
        for (size_t j = 0; j < n_ccys; ++j) {
            if (i != j && rates[i][j] != 0) {
                assert(rates[i][j] > 0);
                graph.set_edge(i, j, -std::log(rates[i][j]));
            }
        }
    }

    // vector of profit -> cycle
    std::vector<std::tuple<double, Cycle>> cycles;
    // avoid duplicates
    std::set<CycleId> found;

    for (auto ccy_source = 0; ccy_source < n_ccys; ++ccy_source) {
        for (auto cycle : g::find_all_negative_cycles(graph, ccy_source)) {
            // already seen ?
            CycleId cycle_id(cycle.begin(), cycle.end());
            if (found.contains(cycle_id)) {
                continue;
            }
            found.insert(cycle_id);

            // now compute the profit
            double final_rate = 1;
            int n = cycle.size();
            for (int idx = 0; idx < n -1; ++idx) {
                auto u = cycle[idx];
                auto v = cycle[idx+1];
                final_rate *= rates[u][v];
            }
            cycles.push_back({(final_rate-1) * 100, cycle});
        }
    }
    // let sort them by decreasing profit
    std::sort(cycles.begin(), cycles.end(), std::greater<>());

    // display the results
    for (const auto& [profit, cycle] : cycles) {
        std::print("Profit: {:.4f} % => ", profit);
        for (int idx = 0; idx < cycle.size() - 1; ++idx) {
            std::print("{} -> ", currencies[cycle[idx]]);
        }
        std::println("{}", currencies[cycle.back()]);
    }
    
    return 0;
}
