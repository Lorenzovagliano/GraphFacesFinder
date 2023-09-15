#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

class Ponto {
public:
    double x, y;

    Ponto(double x_, double y_) : x(x_), y(y_) {}

    Ponto operator - (const Ponto& p) const {
        return Ponto(x - p.x, y - p.y);
    }

    double cross(const Ponto& p) const {
        return x * p.y - y * p.x;
    }

    double cross(const Ponto& p, const Ponto& q) const {
        return (p - *this).cross(q - *this);
    }

    int half() const {
        return int(y < 0 || (y == 0 && x < 0));
    }
};

class Vertice {
public:
    int id;
    Ponto cord;
    std::vector<size_t> lista_adj;

    Vertice() : id(0), cord(Ponto(0, 0)), lista_adj({}) {}

    Vertice(int _id, double _x, double _y) : id(_id), cord(Ponto(_x, _y)), lista_adj({}) {}
};

// Custom comparison function to sort adjacent vertices by polar angle
bool compareByPolarAngle(const Vertice* a, const Vertice* b, const Vertice* reference, const std::vector<Vertice>& vertices) {
    Ponto pl = vertices[a->id].cord - vertices[reference->id].cord;
    Ponto pr = vertices[b->id].cord - vertices[reference->id].cord;
    if (pl.half() != pr.half())
        return pl.half() < pr.half();
    return pl.cross(pr) > 0;
}

// Function to find faces in the graph
std::vector<std::vector<Vertice*>> findFaces(std::vector<Vertice>& vertices) {
    size_t n = vertices.size();
    std::vector<std::vector<size_t>> adj(n);

    // Build adjacency lists for the vertices
    for (size_t i = 0; i < n; i++) {
        auto compare = [&](size_t l, size_t r) {
            Ponto pl = vertices[l].cord - vertices[i].cord;
            Ponto pr = vertices[r].cord - vertices[i].cord;
            if (pl.half() != pr.half())
                return pl.half() < pr.half();
            return pl.cross(pr) > 0;
        };
        std::sort(vertices[i].lista_adj.begin(), vertices[i].lista_adj.end(), compare);
        for (size_t adj_vertex : vertices[i].lista_adj) {
            adj[i].push_back(adj_vertex);
        }
    }

    std::vector<std::vector<bool>> used(n, std::vector<bool>(n, false));
    std::vector<std::vector<Vertice*>> faces;
    for (size_t i = 0; i < n; i++) {
        for (size_t edge_id = 0; edge_id < adj[i].size(); edge_id++) {
            if (used[i][edge_id]) {
                continue;
            }
            std::vector<Vertice*> face;
            size_t v = i;
            size_t e = edge_id;
            while (!used[v][e]) {
                used[v][e] = true;
                face.push_back(&vertices[v]);
                size_t u = adj[v][e];
                size_t e1 = std::lower_bound(adj[u].begin(), adj[u].end(), v, [&](size_t l, size_t r) {
                    Ponto pl = vertices[l].cord - vertices[u].cord;
                    Ponto pr = vertices[r].cord - vertices[u].cord;
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
                double val = face[j]->cord.cross(face[j1]->cord, face[j2]->cord);
                if (val > 0) {
                    sign = 1;
                    break;
                } else if (val < 0) {
                    sign = -1;
                    break;
                }
            }
            face.push_back(face[0]); // Add the first vertex at the end
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

    std::vector<Vertice> vertices;
    int n, m;
    std::cin >> n >> m;
    for(int i = 0; i < n; i++){
        double x, y;
        int grau;
        std::cin >> x >> y >> grau;
        vertices.push_back(Vertice(i, x, y));

        for(int j = 0; j < grau; j++){
            int adjacente;
            std::cin >> adjacente;
            vertices[i].lista_adj.push_back(adjacente);
        }
    }

    std::vector<std::vector<Vertice*>> faces = findFaces(vertices);

    std::cout << faces.size() << '\n';
    for(int i = 0; i < faces.size(); i++){
        for(int j = 0; j < faces[i].size(); j++){
            std::cout << faces[i][j]->id << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}