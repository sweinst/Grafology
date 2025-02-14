#include <grafology/generators/r3mat_generator.h>
#include <cmath>

namespace grafology {
    void R3MatGeneratorBase::generate_degree_distribution(bool is_directed, unsigned n_vertices) {
        double pa, pb, pc, pd;
        unsigned n_edges =
            (unsigned)(2. / 3. * n_vertices * std::log(n_vertices) + 0.38481 * n_vertices);
        // initialize array: each node have degree 1
        _degrees.clear();
        _degrees.resize(n_vertices, 1);
        // adjust the remaining number of edges to generate
        if (is_directed) {
            n_edges -= n_vertices;
        } else {
            if (n_vertices % 2 == 0) {
                _degrees[0] = 2;
            }
            n_edges -= (n_vertices + 1) / 2;
        }
        // R3-MAT probabilities
        if (is_directed) {
            pa = 0.75;
            pb = 0.05;
            pc = 0.19;
            pd = 0.01;
        } else {
            pa = 0.75;
            pb = 0.05;
            pc = 0.18;
            pd = 0.02;
        }
        double depth = std::ceil(std::log2(n_vertices));
        _offset1 = (0.25 - pa) / depth;
        _offset2 = (0.25 - pb) / depth;
        _offset3 = (0.25 - pc) / depth;
        _offset4 = (0.25 - pd) / depth;

        edge_t e;
        for (unsigned i = 0; i < n_edges; i++) {
            do {
                e = choose_edge(0, 0, n_vertices - 1, n_vertices - 1, pa, pb, pc, pd);
            } while (_degrees[e.start] >= n_vertices - 1 || _degrees[e.end] >= n_vertices - 1);
            _degrees[e.start]++;
            _degrees[e.end]++;
        }
    }

    edge_t R3MatGeneratorBase::choose_edge(
        unsigned x1,
        unsigned y1,
        unsigned xn,
        unsigned yn,
        double a,
        double b,
        double c,
        double d
    ) {
        double r = _rnd_gen(_rd);
        unsigned mx, my;

        if ((xn - x1) == 0 && (yn - y1) == 0) {
            mx = x1;
            my = y1;
            return {mx, my};
        }
        if ((xn - x1) == 0 && (yn - y1) == 1) {
            if (r < 0.5) {
                mx = x1;
                my = y1;
            } else {
                mx = x1;
                my = yn;
            }
            return {mx, my};
        }
        if ((xn - x1) == 1 && (yn - y1) == 0) {
            if (r < 0.5) {
                mx = x1;
                my = y1;
            } else {
                mx = xn;
                my = yn;
            }
            return {mx, my};
        }

        double ab = a + b;
        double abc = a + b + c;
        double new_a = std::abs(a + _offset1);
        double new_b = std::abs(b + _offset2);
        double new_c = std::abs(c + _offset3);
        double new_d = std::abs(d + _offset4);
        unsigned half_x;
        unsigned half_y;
        if (r < a) {
            half_x = (unsigned)std::floor((x1 + xn) / 2);
            half_y = (unsigned)std::floor((y1 + yn) / 2);
            return choose_edge(x1, y1, half_x, half_y, new_a, new_b, new_c, new_d);
        } else if (r >= a && r < ab) {
            half_x = (unsigned)std::ceil((x1 + xn) / 2);
            half_y = (unsigned)std::floor((y1 + yn) / 2);
            return choose_edge(half_x, y1, xn, half_y, new_a, new_b, new_c, new_d);
        } else if (r >= ab && r < abc) {
            half_x = (unsigned)std::floor((x1 + xn) / 2);
            half_y = (unsigned)std::ceil((y1 + yn) / 2);
            return choose_edge(x1, half_y, half_x, yn, new_a, new_b, new_c, new_d);
        }
        half_x = (unsigned)std::ceil((x1 + xn) / 2);
        half_y = (unsigned)std::ceil((y1 + yn) / 2);
        return choose_edge(half_x, half_y, xn, yn, new_a, new_b, new_c, new_d);
    }
    
    generator<edge_t> R3MatGeneratorBase::generate_directed_edges() {
        unsigned n_vertices = _degrees.size();
        for (vertex_t start = 0; start < n_vertices; ++start) {
            auto degree = _degrees[start];
            int end = start - 1;
            while ((degree > 0) && (end >= 0)) {
                co_yield {start, static_cast<vertex_t>(end)};
                --degree;
                --end;
            }
            end = start + 1;
            while ((degree > 0) && (end < n_vertices)) {
                co_yield {start, static_cast<vertex_t>(end)};
                --degree;
                ++end;
            }
            assert(degree == 0);
        }
    }
    
    generator<edge_t> R3MatGeneratorBase::generate_undirected_edges() {
        unsigned n_vertices = _degrees.size();
        for (vertex_t start = 0; start < n_vertices; ++start) {
            auto end = start + 1;
            while (end < n_vertices) {
                if (_degrees[start] == 0) {
                    break;
                }
                co_yield {start, end};
                _degrees[start]--;
                co_yield {end, start};
                _degrees[end]--;
                ++end;
            }
            assert(_degrees[start] == 0);
        }
    }
} // namespace grafology

