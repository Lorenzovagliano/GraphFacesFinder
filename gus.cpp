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

// Slope of the line from point p to point q.
long double RelativeSlope(const Ponto& p, const Ponto& q) {
    return atan2l(q.y - p.y, q.x - p.x);
}

// Compare function to sort pontos counterclockwise with respect to B->A
bool CompareByPolarAngle(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double angleA = RelativeSlope(B, A);
    double angle1 = RelativeSlope(B, a) - angleA;
    double angle2 = RelativeSlope(B, b) - angleA;

    // Ensure angles are in the range [0, 2*pi] for correct sorting
    if (angle1 < 0) angle1 += 2 * M_PI;
    if (angle2 < 0) angle2 += 2 * M_PI;

    return angle1 < angle2;
}

// Function to sort a vector of pontos counterclockwise
void sortCounterclockwise(Vertice* A, Vertice* B) {
    std::vector<Ponto> pontos;
    std::vector<Aresta*> arestas;

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        pontos.push_back(B->lista_arestas[i]->v2->cord);
    }

    std::sort(pontos.begin(), pontos.end(), [&](const Ponto& a, const Ponto& b) {
        return CompareByPolarAngle(a, b, A->cord, B->cord);
    });

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        for (int j = 0; j < B->lista_arestas.size(); j++) {
            if (pontos[i].x == B->lista_arestas[j]->v2->cord.x && pontos[i].y == B->lista_arestas[j]->v2->cord.y) {
                arestas.push_back(B->lista_arestas[j]);
                break;
            }
        }
    }

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        B->lista_arestas[i] = arestas[i];
    }
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
        std::cout << vertices[i]->id << ": ";
        for(int j = 0; j < vertices[i]->lista_adj.size(); j++){
            std::cout << vertices[i]->lista_adj[j]->id << ' ';

            Aresta* nova = (new Aresta(vertices[i], vertices[listasAdjacencia[i][j]]));
            arestas.push_back(nova);
            vertices[i]->lista_arestas.push_back(nova);
        }
        std::cout << '\n';
    }
    std::cout << '\n';


    std::vector<std::vector<Vertice*>> faces;

    Aresta* inicial = arestas[13];
    Aresta* atual = inicial;

    std::vector<Vertice*> face;

    face.push_back(inicial->v1);
    std::cout << "Adicionando " << inicial->v1->id << '\n';
    while (atual->visitada != true) {
        std::cout << "Aresta atual: (" << atual->v1->id << ", " << atual->v2->id << ")\n";
        face.push_back(atual->v2);
        std::cout << "Adicionando " << atual->v2->id << '\n';
        atual->visitada = true;

        sortCounterclockwise(atual->v1, atual->v2);

        std::cout << atual->v2->id << ": ";
        for (int i = 0; i < atual->v2->lista_arestas.size(); i++) {
            std::cout << atual->v2->lista_arestas[i]->v2->id << ", ";
        }
        std::cout << '\n';

        // Find the next unvisited edge with the smallest polar angle
        for (int j = 0; j < atual->v2->lista_arestas.size(); j++) {
            if (atual->v2->lista_arestas[j]->v2 != atual->v1) {
                atual = atual->v2->lista_arestas[j];
                break;
            }
            else if(atual->v2->lista_arestas.size() < 2){
                atual = atual->v2->lista_arestas[j];
                break;
            }

        }
    }
    faces.push_back(face);


    for (int i = 0; i < faces.size(); i++) {
        std::cout << "Face " << i + 1 << ": ";
        for (int j = 0; j < faces[i].size(); j++) {
            std::cout << faces[i][j]->id << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
