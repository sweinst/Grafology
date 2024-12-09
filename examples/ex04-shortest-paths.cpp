// example of the use of the all_shortest_paths algorithm (Djikstra's algorithm)
// it's try to find the shortest path between two stations in the London Tube network

// the main issue is that there is only one edge/line between two vertices/stations
// as many lines share the segments of the networks, the line indication can be misleading
// also some different routes can have the same distance and the less practical one can be chosen

#include <grafology/algorithms/all_shortest_paths.h>
#include <grafology/algorithms/shortest_path.h>
#include "london_tube.h"
#include "utils.h"

namespace g = grafology;
namespace lt = LondonTube;
using namespace std::string_literals;
using Graph = g::UndirectedSparseGraph<lt::Station>;

class Program {
 public:
  Program(int argc, const char* argv[]);

  operator bool() const { return _ok; }

  void run_dijkstra() const;
  void run_a_star() const;

 private:
  void print_path(generator<g::Step<lt::Station>>& path) const;

  bool _ok = true;
  lt::Station _start;
  std::vector<lt::Station> _destinations;
  g::UndirectedSparseGraph<lt::Station> _tube;
};

Program::Program(int argc, const char* argv[])
    : _ok(true)
    , _tube(lt::stations.size()) {
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
  auto s = lt::Station::from_name(start);
  if (s) {
    _start = *s;
  } else {
    std::println("Unable to find the station '{}'", start);
    _ok = false;
  }
  for (const auto& dest : destinations) {
    auto d = lt::Station::from_name(dest);
    if (d) {
      _destinations.push_back(*d);
    } else {
      std::println("Unable to find the station '{}'", dest);
      _ok = false;
    }
  }

  _tube.add_vertices(lt::stations);
  _tube.set_edges(lt::connections);
}

void Program::run_dijkstra() const {
  print_frame("Dijkstra's algorithm");
  auto paths = g::all_shortest_paths(_tube, _start);
  for (const auto& dest : _destinations) {
    std::println("===> {} to {}", _start._name, dest._name);
    if (!paths.is_reachable(dest)) {
      std::println("stderr: Cannot find a path to {}", dest._name);
    } else {
      auto g = paths.get_path(dest);
      print_path(g);
    }
  }
}

void Program::run_a_star() const {
  print_frame("A* algorithm");

  auto cost_function = [](const lt::Station& a, const lt::Station& b) -> double {
    return a.distance_from(b);
  };

  for (const auto& dest : _destinations) {
    std::println("===> {} to {}", _start._name, dest._name);
    auto path = g::shortest_path(_tube, _start, dest, cost_function);
    if (!path.is_reachable()) {
      std::println("stderr: Cannot find a path to {}", dest._name);
    } else {
      auto g = path.get_path();
      print_path(g);
    }
  }
}

void Program::print_path(generator<g::Step<lt::Station>>& path) const {
  int prev_line = -1;
  g::vertex_t prev_station_id = _start._id;
  std::string initial_station = _start._name;
  std::string prev_station = _start._name;
  double d_kms = 0.;
  for (const auto& [v, d] : path) {
    d_kms = d / 1000.;
    if (v != _start) {
      auto connection = lt::Connection::from_id(prev_station_id, v._id);
      if (connection->line != prev_line) {
        auto line_details = lt::Line::from_id(prev_line);
        if (prev_line != -1) {
          std::println(
              "{} => {} (line: {}, distance: {:.1f} km(s))", initial_station, prev_station,
              line_details->_name, d_kms
          );
          initial_station = prev_station;
        }
        prev_line = connection->line;
      }
      prev_station = v._name;
    }
    prev_station_id = v._id;
  }
  auto line = lt::Line::from_id(prev_line)->_name;
  std::println(
      "{} => {} (line: {}, distance: {:.1f} km(s))", initial_station, prev_station, line, d_kms
  );
  std::println();
}

int main(int argc, const char* argv[]) {
  Program program(argc, argv);
  if (!program) {
    return 1;
  }
  program.run_dijkstra();
  std::println();
  program.run_a_star();
  return 0;
}
