#pragma once
#include <grafology/grafology.h>
#include <map>
#include <set>

// a graph of London Tube stations and connections
// derived from data copied from https://markd.ie/2016/04/10/The-London-Tube-as-a-Graph/

namespace LondonTube {
    namespace g = grafology;

    struct Line {
        g::vertex_t _id;
        std::string _name {};
        unsigned _color = 0;

        auto operator <=>(const Line& l) const noexcept {
            return _id <=> l._id;
        }
    };
    extern std::set<Line> lines;

    struct Station{
        g::vertex_t _id;
        std::string _name {};
        double _latitude = 0;
        double _longitude = 0;
        // some stations are in zones 2 AND 3! => station = 2.5
        double _zone = 0;
        unsigned _total_lines = 0;
        bool is_railways_station = false;

        auto operator <=>(const Station& s) const noexcept {
            return _id <=> s._id;
        }
    };

    extern std::set<Station> stations;

    struct Connection : public g::edge_t {
        unsigned line = 0;

        std::strong_ordering operator <=>(const Connection& c) const noexcept {
            auto cmp = start <=> c.start;
            if (cmp != std::strong_ordering::equal)  {
                return cmp;
            }
            return end <=> c.end;
        }
    };
    
    extern std::set<Connection> connections;

}  // namespace LondonTube

namespace std {
    namespace lt = LondonTube;

    template <>
    struct hash<lt::Station> {
        std::size_t operator() (const lt::Station& v) const {
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
}
