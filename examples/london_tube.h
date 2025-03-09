#pragma once
#include <grafology/graph.h>
#include <map>
#include <optional>
#include <set>

// a graph of London Tube stations and connections
// derived from data found at
// https://www.whatdotheyknow.com/request/station_to_station_journey_times

namespace LondonTube {
namespace g = grafology;

struct Line {
  unsigned _id;
  std::string _name{};

  auto operator<=>(const Line& l) const noexcept { return _id <=> l._id; }
  bool operator==(const Line& l) const noexcept { return _id == l._id; }

  static std::optional<Line> from_id(unsigned id);
};
extern std::set<Line> lines;

struct Station {
  g::vertex_t _id;
  std::string _name{};
  double _latitude = 0;
  double _longitude = 0;

  auto operator<=>(const Station& s) const noexcept { return _id <=> s._id; }
  bool operator==(const Station& s) const noexcept { return _id == s._id; }

  static std::optional<Station> from_id(g::vertex_t id);
  static std::optional<Station> from_name(const std::string& name);

  double distance_from(const Station& s) const;
};

extern std::set<Station> stations;
}  // namespace LondonTube

namespace std {
namespace lt = LondonTube;

template <>
struct hash<lt::Station> {
  std::size_t operator()(const lt::Station& v) const {
    return std::hash<int>{}(v._id);
  }
};

template <>
struct formatter<lt::Station, char> {
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <class FormatContext>
  auto format(const lt::Station& v, FormatContext& ctx) const {
    return std::format_to(ctx.out(), "[{}: {}]", v._name, v._id);
  }
};
}  // namespace std

namespace LondonTube {
struct Connection : public g::EdgeDefinition<Station, double> {
  // the weight os the distance in kms
  unsigned line = 0;
  double time_unimpeded_sec;
  double time_peak_am_sec;
  double time_inter_peak_sec;

  Connection(g::vertex_t start,
             g::vertex_t end,
             double distance_km = 0,
             unsigned line = 0,
             double u = 0,
             double p = 0,
             double i = 0)
      : g::EdgeDefinition<Station, double>{{start},
                                   {end},
                                   weight_lt(distance_km * 1'000)},
        line{line},
        time_unimpeded_sec(u),
        time_peak_am_sec(p),
        time_inter_peak_sec(i) {}

  std::strong_ordering operator<=>(const Connection& c) const noexcept {
    auto cmp = start <=> c.start;
    if (cmp != std::strong_ordering::equal) {
      return cmp;
    }
    return end <=> c.end;
  }

  static std::optional<Connection> from_id(g::vertex_t start, g::vertex_t end);
};

extern std::set<Connection> connections;

}  // namespace LondonTube
