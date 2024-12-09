// example of the use of the all_shortest_paths algorithm (Djikstra's algorithm)
// it's try to find the shortest path between two stations in the London Tube network

// the main issue is that there is only one edge/line between two vertices/stations
// as many lines share the segments of the networks, the line indication can be misleading
// also some different routes can have the same distance and the less practical one can be chosen

#include <grafology/algorithms/all_shortest_paths.h>
#include <print>
#include "london_tube.h"

namespace g = grafology;
namespace lt = LondonTube;
using namespace std::string_literals;
using Graph =   g::UndirectedSparseGraph<lt::Station>;

void print_path(const Graph& g, generator<lt::Station> path, const lt::Station& start) {
  int prev_line = -1;
  g::vertex_t prev_station_id = start._id;
  std::string initial_station = start._name;
  std::string prev_station = start._name;
  for (auto v : path) {
    if (v != start) {
      auto connection = lt::Connection::from_id(prev_station_id, v._id);
      if (connection->line != prev_line) {
        auto line_details = lt::Line::from_id(prev_line);
        if (prev_line != -1) {
          std::println("{} => {} ({})", initial_station, prev_station, line_details->_name);
          initial_station = prev_station;
        }
        prev_line = connection->line;
      }
      prev_station = v._name;
    }
    prev_station_id = v._id;
  }
  auto line = lt::Line::from_id(prev_line)->_name;
  std::println(stderr, "{} => {} ({})", initial_station, prev_station, line);
}

int main(int argc, const char* argv[]) {
  using namespace LondonTube;

  std::string start = "Amersham"s;
  std::vector<std::string> destinations = {"Baker Street"s, "Morden", "Dagenham East"s};
  if (argc > 1) {
    start = argv[1];
    if (argc > 2) {
      destinations.clear();
      for (int i = 2; i < argc; ++i) {
        destinations.push_back(argv[i]);
      }
    }
  }

  g::UndirectedSparseGraph<Station> tube(stations.size());
  tube.add_vertices(stations);
  tube.set_edges(connections);

  auto s = Station::from_name(start);
  if (!s) {
    std::println("Station '{}' not found", start);
    return 1;
  }

  auto paths = g::all_shortest_paths(tube, *s);

//   for (const auto& dest : destinations) {
//     auto d = Station::from_name(dest);
//     if (d) {
//       std::println("===> {} to {}", s->_name, d->_name);
//       if (!paths.is_reachable(*d)) {
//         std::println("stderr: Cannot find a path to {}", d->_name);
//       } else {
//         std::println("Distance to {} is {:03} km(s)", d->_name, paths.get_distance(*d) / 1000.);
//         print_path(tube, paths.get_path(*d), *s);
//       }
//     } else {
//       std::println("Station '{}' not found", dest);
//     }
//   }

  return 0;
}
