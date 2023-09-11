#include <iostream>
#include <vector>
#include <algorithm>

struct Point {
    int64_t x, y;

    Point(int64_t x_, int64_t y_): x(x_), y(y_) {}

    Point operator - (const Point & p) const {
        return Point(x - p.x, y - p.y);
    }

    int64_t cross (const Point & p) const {
        return x * p.y - y * p.x;
    }

    int64_t cross (const Point & p, const Point & q) const {
        return (p - *this).cross(q - *this);
    }

    int half () const {
        return int(y < 0 || (y == 0 && x < 0));
    }
};

std::vector<std::vector<size_t>> find_faces(std::vector<Point> vertices, std::vector<std::vector<size_t>> adj) {
    size_t n = vertices.size();
    std::vector<std::vector<char>> used(n);
    for (size_t i = 0; i < n; i++) {
        used[i].resize(adj[i].size());
        used[i].assign(adj[i].size(), 0);
        auto compare = [&](size_t l, size_t r) {
            Point pl = vertices[l] - vertices[i];
            Point pr = vertices[r] - vertices[i];
            if (pl.half() != pr.half())
                return pl.half() < pr.half();
            return pl.cross(pr) > 0;
        };
        std::sort(adj[i].begin(), adj[i].end(), compare);
    }
    std::vector<std::vector<size_t>> faces;
    for (size_t i = 0; i < n; i++) {
        for (size_t edge_id = 0; edge_id < adj[i].size(); edge_id++) {
            if (used[i][edge_id]) {
                continue;
            }
            std::vector<size_t> face;
            size_t v = i;
            size_t e = edge_id;
            while (!used[v][e]) {
                used[v][e] = true;
                face.push_back(v);
                size_t u = adj[v][e];
                size_t e1 = std::lower_bound(adj[u].begin(), adj[u].end(), v, [&](size_t l, size_t r) {
                    Point pl = vertices[l] - vertices[u];
                    Point pr = vertices[r] - vertices[u];
                    if (pl.half() != pr.half())
                        return pl.half() < pr.half();
                    return pl.cross(pr) > 0;
                }) - adj[u].begin() + 1;
                if (e1 == adj[u].size()) {
                    e1 = 0;
                }
                v = u;
                e = e1;
            }
            std::reverse(face.begin(), face.end());
            int sign = 0;
            for (size_t j = 0; j < face.size(); j++) {
                size_t j1 = (j + 1) % face.size();
                size_t j2 = (j + 2) % face.size();
                int64_t val = vertices[face[j]].cross(vertices[face[j1]], vertices[face[j2]]);
                if (val > 0) {
                    sign = 1;
                    break;
                } else if (val < 0) {
                    sign = -1;
                    break;
                }
            }
            if (sign <= 0) {
                faces.insert(faces.begin(), face);
            } else {
                faces.emplace_back(face);
            }
        }
    }
    return faces;
}

int main() {
    // Create a simple example of a planar connected graph with vertices and edges
    std::vector<Point> vertices = {
        {0, 0},
        {1, 1},
        {1, -1},
        {2, 0},
        {4, 0},
        {4, -1},
        {-3, 0},
        {-2, 0}
    };

    // Define the adjacency list for the graph
    std::vector<std::vector<size_t>> adjacency = {
        {2, 2, 3},
        {4, 1, 4, 5, 7},
        {5, 1, 4, 5, 6, 7},
        {2, 2, 3},
        {3, 2, 3, 6},
        {2, 3, 5},
        {3, 2, 3, 8},
        {1, 7}
    };

    // Call the find_faces function to find the faces of the graph
    std::vector<std::vector<size_t>> faces = find_faces(vertices, adjacency);

    // Print the faces
    std::cout << "Faces of the graph:" << std::endl;
    for (size_t i = 0; i < faces.size(); ++i) {
        std::cout << "Face " << i + 1 << ": ";
        for (size_t j = 0; j < faces[i].size(); ++j) {
            std::cout << faces[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}