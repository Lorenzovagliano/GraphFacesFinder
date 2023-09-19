#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>

class Ponto{
    public:
        long double x, y;

        Ponto(){
            this->x = 0;
            this->y = 0;
        }

        Ponto(long double _x, long double _y){
            this->x = _x;
            this->y = _y;
        }

        Ponto operator-(const Ponto& other) const {
            return Ponto(x - other.x, y - other.y);
        }
};

class angle_sort{
    Ponto m_origin;
    Ponto m_dreference;

    // z-coordinate of cross-product, aka determinant
    static double xp(Ponto a, Ponto b) { return a.x * b.y - a.y * b.x; }
    public:
        angle_sort(const Ponto origin, const Ponto reference) : m_origin(origin), m_dreference(reference - origin) {}
        bool operator()(const Ponto a, const Ponto b) const
        {
            const Ponto da = a - m_origin, db = b - m_origin;
            const double detb = xp(m_dreference, db);

            // nothing is less than zero degrees
            if (detb == 0 && db.x * m_dreference.x + db.y * m_dreference.y >= 0) return false;

            const double deta = xp(m_dreference, da);

            // zero degrees is less than anything else
            if (deta == 0 && da.x * m_dreference.x + da.y * m_dreference.y >= 0) return true;

            if (deta * detb >= 0) {
                // both on same side of reference, compare to each other
                return xp(da, db) > 0;
            }

            // vectors "less than" zero degrees are actually large, near 2 pi
            return deta > 0;
        }
};

class Aresta;

class Vertice{
    public:
        int id;
        Ponto cord;

        std::vector<Vertice*> lista_adj;
        std::vector<Aresta*> lista_arestas;

        Vertice(){
            this->id = 0;
            this->cord = Ponto();
        }

        Vertice(int _id, long double _x, long double _y){
            this->id = _id;
            this->cord = Ponto(_x, _y);
        }

};

class Aresta{
    public:
        Vertice* v1;
        Vertice* v2;
        bool visitada;

        Aresta(){
            this->v1 = nullptr;
            this->v2 = nullptr;
            this->visitada = false;
        }

        Aresta(Vertice* _v1, Vertice* _v2){
            this->v1 = _v1;
            this->v2 = _v2;
            this->visitada = false;
        }
};

// Euclidean distance between two points.
double Distance(const Ponto& a, const Ponto& b) {
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}

// Slope of the line passing through the origin and point p.
double Slope(const Ponto& p) {
    return atan2(p.y, p.x);
}

// Relative slope of the line from point p to point q.
double RelativeSlope(const Ponto& p, const Ponto& q) {
    return atan2(q.y - p.y, q.x - p.x);
}

// Determine if we are making a left turn, right turn, or going straight
int TurnType(const Ponto& a, const Ponto& b, const Ponto& c) {
    double v = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    if (v < 0) return -1; // left turn.
    if (v > 0) return +1; // right turn.
    return 0; // straight.
}

// Function to compare points by their relative polar angle to Ponto B and A
bool CompareByPolarAngle(const Ponto& a, const Ponto& b, const Ponto& originA, const Ponto& originB) {
    double angleA = RelativeSlope(originB, a);
    double angleB = RelativeSlope(originB, b);
    return angleA < angleB;
}

// Function to sort a vector of points counterclockwise
void sortCounterclockwise(Vertice* B, Vertice* A) {
    std::vector<Ponto> pontos;
    std::vector<Aresta*> arestas;

    for(int i = 0; i < B->lista_arestas.size(); i++){
        pontos.push_back(B->lista_arestas[i]->v2->cord);
    }

    std::sort(pontos.begin(), pontos.end(), [&](const Ponto& a, const Ponto& b) {
        return CompareByPolarAngle(a, b, A->cord, B->cord);
    });

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        for (int j = 0; j < B->lista_arestas.size(); j++){
            if(pontos[i].x == B->lista_arestas[j]->v2->cord.x && pontos[i].y == B->lista_arestas[j]->v2->cord.y){
                arestas.push_back(B->lista_arestas[j]);
            }
        }
    }

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        B->lista_arestas[i] = arestas[i];
    }
}

void sortAngle(Vertice* v1, Vertice* v2) {
    Ponto reference = Ponto(v1->cord.x, v2->cord.x);
    std::vector<Ponto> pontos;
    std::vector<Aresta*> arestas;
    
    for (int i = 0; i < v2->lista_arestas.size(); i++) {
        pontos.push_back(v2->lista_arestas[i]->v2->cord);
    }
    
    // Sort both pontos and arestas based on the custom comparator
    std::sort(pontos.begin(), pontos.end(), angle_sort(v2->cord, reference));
    
    // Rearrange lista_arestas based on the sorted order of pontos
    for (int i = 0; i < v2->lista_arestas.size(); i++) {
        for (int j = 0; j < v2->lista_arestas.size(); j++){
            if(pontos[i].x == v2->lista_arestas[j]->v2->cord.x && pontos[i].y == v2->lista_arestas[j]->v2->cord.y){
                arestas.push_back(v2->lista_arestas[j]);
            }
        }
    }

    for (int i = 0; i < v2->lista_arestas.size(); i++) {
        v2->lista_arestas[i] = arestas[i];
    }
}

long double inclinacaoRelativa(Ponto p, Ponto q) {
    return atan2l(q.y - p.y, q.x - p.x);
}

void orderAdjacentEdgesByAngle(Vertice* v1, Vertice* v2) {
    // Calculate the direction vector of the incoming edge (v1, v2)
    Ponto incomingDir(v2->cord.x - v1->cord.x, v2->cord.y - v1->cord.y);

    // Create a vector of pairs to associate angles with edges
    std::vector<std::pair<long double, Aresta*>> anglesAndEdges;

    for (int j = 0; j < v2->lista_arestas.size(); j++) {
        // Calculate the direction vector of the current adjacent edge
        Ponto edgeDir(v2->lista_arestas[j]->v2->cord.x - v2->cord.x,
                      v2->lista_arestas[j]->v2->cord.y - v2->cord.y);

        // Calculate the dot product between the incoming edge and the current edge
        long double dotProduct = incomingDir.x * edgeDir.x + incomingDir.y * edgeDir.y;

        // Calculate the magnitude of the vectors
        long double incomingMag = sqrt(incomingDir.x * incomingDir.x + incomingDir.y * incomingDir.y);
        long double edgeMag = sqrt(edgeDir.x * edgeDir.x + edgeDir.y * edgeDir.y);

        // Calculate the angle between the vectors using the dot product
        long double angle = acos(dotProduct / (incomingMag * edgeMag));

        // Store the angle and the corresponding edge in the vector
        anglesAndEdges.push_back(std::make_pair(angle, v2->lista_arestas[j]));
    }

    // Sort the vector of pairs based on angles in ascending order
    std::sort(anglesAndEdges.begin(), anglesAndEdges.end(),
              [](const std::pair<long double, Aresta*>& a, const std::pair<long double, Aresta*>& b) {
                  return a.first < b.first;
              });

    // Update v2's lista_arestas based on the sorted vector of pairs
    for (int j = 0; j < anglesAndEdges.size(); j++) {
        v2->lista_arestas[j] = anglesAndEdges[j].second;
    }

    std::reverse(v2->lista_arestas.begin(), v2->lista_arestas.end());
}


int main(){

    std::vector<Vertice*> vertices;
    std::vector<std::vector<int>> listasAdjacencia;

    int n, m;
    std::cin >> n >> m;
    listasAdjacencia.resize(n);
    for(int i = 0; i < n; i++){
        long double x, y;
        int grau;
        std::cin >> x >> y >> grau;
        vertices.push_back(new Vertice(i + 1, x, y));

        for(int j = 0; j < grau; j++){
            int adjacente;
            std::cin >> adjacente;
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    }

    for (int i = 0; i < vertices.size(); i++){
        for (int j = 0; j < listasAdjacencia[i].size(); j++){
            vertices[i]->lista_adj.push_back(vertices[listasAdjacencia[i][j]]);
        }
    }

    std::vector<Aresta*> arestas;

    for(int i = 0; i < vertices.size(); i++){
        for(int j = 0; j < vertices[i]->lista_adj.size(); j++){

            Aresta* nova = (new Aresta(vertices[i], vertices[listasAdjacencia[i][j]]));
            arestas.push_back(nova);
            vertices[i]->lista_arestas.push_back(nova);
        }
    }


    std::vector<std::vector<Vertice*>> faces;

    Aresta* inicial = arestas[0];

    while (inicial->visitada != true) {
        Aresta* atual = inicial;
        std::vector<Vertice*> face;

        while (atual->visitada != true) {
            face.push_back(atual->v1);
            atual->visitada = true;

            sortCounterclockwise(atual->v1, atual->v2);

            // Find the next unvisited edge with the smallest polar angle
            for (int j = 0; j < atual->v2->lista_arestas.size(); j++) {
                if (atual->v2->lista_arestas[j]->v2 != atual->v1) {
                    atual = atual->v2->lista_arestas[j];
                    break;
                }
                else if (atual->v2->lista_arestas.size() < 2) {
                    atual = atual->v2->lista_arestas[j];
                    break;
                }
            }
        }

        // Add the initial vertex to close the face
        face.push_back(inicial->v1);
        faces.push_back(face);

        // Find the next unvisited edge to start a new face
        for (Aresta* a : arestas) {
            if (!a->visitada) {
                inicial = a;
                break;
            }
        }
    }

    std::cout << faces.size() << '\n';
    for (int i = 0; i < faces.size(); i++) {
        std::cout << faces[i].size() << ' ';
        for (int j = 0; j < faces[i].size(); j++) {
            std::cout << faces[i][j]->id << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
