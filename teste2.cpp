#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stack>

class Ponto{
    public:
        double x, y;

        Ponto(){
            this->x = 0;
            this->y = 0;
        }

        Ponto(int _x, int _y){
            this->x = _x;
            this->y = _y;
        }
};

class Aresta;

class Vertice{
    public:
        int id;
        Ponto cord;
        int grau;

        std::vector<Vertice*> lista_adj;
        std::vector<Aresta*> lista_arestas;
        std::string cor;

        Vertice(){
            this->id = 0;
            this->cord = Ponto();
            this->grau = 0;
            this->cor = "branco";
        }

        Vertice(int _id, int _x, int _y, int _grau){
            this->id = _id;
            this->cord = Ponto(_x, _y);
            this->grau = _grau;
            this->cor = "branco";
        }

        Vertice(int _id, int _x, int _y, int _grau, std::vector<Vertice*> _lista_adj){
            this->id = _id;
            this->cord = Ponto(_x, _y);
            this->grau = _grau;
            this->lista_adj = _lista_adj;
            this->cor = "branco";
        }

};

class Aresta{
    public:
        Vertice* v1;
        Vertice* v2;
        int visitas = 0;

        Aresta(){
            this->v1 = nullptr;
            this->v2 = nullptr;
            this->visitas = 0;
        }

        Aresta(Vertice* _v1, Vertice* _v2){
            this->v1 = _v1;
            this->v2 = _v2;
            this->visitas = 0;
        }
};

// Dist^ancia euclidiana de a para b.
double distancia(Ponto a, Ponto b) {
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}

// Coeficiente da reta que passa na origem e p.
double inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}
// Coeficiente da reta orientada de p para q.
double inclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}

// Custom comparison function to sort adjacent vertices by polar angle
bool compareByPolarAngle(const Vertice* a, const Vertice* b, const Vertice* reference) {
    double angle_a = atan2(a->cord.y - reference->cord.y, a->cord.x - reference->cord.x);
    double angle_b = atan2(b->cord.y - reference->cord.y, b->cord.x - reference->cord.x);
    return angle_a < angle_b;
}

void sortAdjacentVerticesAndEdgesByPolarAngle(Vertice* reference, Vertice* ordered) {
    // Create a vector to store pairs of adjacent vertices and corresponding edges
    std::vector<std::pair<Vertice*, Aresta*>> adjAndEdges;

    for (int i = 0; i < ordered->lista_adj.size(); ++i) {
        Vertice* adj = ordered->lista_adj[i];
        Aresta* edge = ordered->lista_arestas[i]; // Corresponding edge

        adjAndEdges.push_back(std::make_pair(adj, edge));
    }

    // Sort the adjacent vertices and corresponding edges by polar angle
    std::sort(adjAndEdges.begin(), adjAndEdges.end(),
              [reference](const std::pair<Vertice*, Aresta*>& a, const std::pair<Vertice*, Aresta*>& b) {
                  return compareByPolarAngle(a.first, b.first, reference);
              });

    // Update the ordered vertices and edges
    for (int i = 0; i < adjAndEdges.size(); ++i) {
        ordered->lista_adj[i] = adjAndEdges[i].first;
        ordered->lista_arestas[i] = adjAndEdges[i].second;
    }
}

void DFS(Vertice* raiz){
    raiz->cor = "cinza";
    std::cout << raiz->id << " Ficou cinza\n";
    for(int i = 0; i < raiz->lista_adj.size(); i++){
        if(raiz->lista_adj[i]->cor == "branco"){
            DFS(raiz->lista_adj[i]);
        }
    }
    raiz->cor = "preto";
    std::cout << raiz->id << " Ficou preto\n";
}

void acharFacesDFS(Aresta* aresta, std::vector<Vertice*> &face){
    if(aresta->visitas > 0){
        return;
    }

    face.push_back(aresta->v1);
    std::cout << "adicionando " << aresta->v1->id << '\n';
    aresta->visitas++;

    for(int i = 0; i < aresta->v2->lista_arestas.size(); i++){
        sortAdjacentVerticesAndEdgesByPolarAngle(aresta->v2, aresta->v2->lista_adj[i]);

        std::cout << aresta->v2->id << ": \n";
        for(int j = 0; j < aresta->v2->lista_adj.size(); j++){
            std::cout << aresta->v2->lista_adj[j]->id << ' ';
        }
        std::cout << '\n';

        if((aresta->v2->lista_arestas[i]->visitas == 0) && (aresta->v2->lista_adj[i] != aresta->v1)){
            acharFacesDFS(aresta->v2->lista_arestas[i], face);
        }
    }
    return;
}

void acharFaces(std::vector<Aresta*> arestas, std::vector<std::vector<Vertice*>> &faces){
    for(int i = 0; i < arestas.size(); i++){
        if(arestas[i]->visitas == 0){
            std::vector<Vertice*> face;
            acharFacesDFS(arestas[i], face);
            faces.push_back(face);
        }
    }
}

int main(){
    std::vector<Vertice*> vertices = {new Vertice(0, 0, 0, 2), new Vertice(1, 1, 0, 2), new Vertice(2, 1, 1, 2), new Vertice(3, 0, 1, 2)};
    vertices[0]->lista_adj = {vertices[1], vertices[3]};
    vertices[0]->lista_arestas.push_back(new Aresta(vertices[0], vertices[1]));
    vertices[0]->lista_arestas.push_back(new Aresta(vertices[0], vertices[3]));

    vertices[1]->lista_adj = {vertices[0], vertices[2]};
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[0]));
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[2]));

    vertices[2]->lista_adj = {vertices[1], vertices[3]};
    vertices[2]->lista_arestas.push_back(new Aresta(vertices[2], vertices[1]));
    vertices[2]->lista_arestas.push_back(new Aresta(vertices[2], vertices[3]));

    vertices[3]->lista_adj = {vertices[2], vertices[0]};
    vertices[3]->lista_arestas.push_back(new Aresta(vertices[3], vertices[2]));
    vertices[3]->lista_arestas.push_back(new Aresta(vertices[3], vertices[0]));

    std::vector<Aresta*> arestas;
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < vertices[i]->lista_adj.size(); j++) {
            // Check if the vertex has associated edges
            if (vertices[i]->lista_arestas.size() > j) {
                arestas.push_back(vertices[i]->lista_arestas[j]);
                std::cout << vertices[i]->id << ' ' << vertices[i]->lista_adj[j]->id << '\n';
            }
        }
        std::cout << '\n';
    }

    /*for(int i = 0; i < arestas.size(); i++){
        std::cout << arestas[i]->visitas << '\n';
    }*/

    std::vector<std::vector<Vertice*>> faces;
    acharFaces(arestas, faces);

    // Print the found faces
    for (const std::vector<Vertice*>& face : faces) {
        std::cout << "Face:";
        for (Vertice* vertex : face) {
            std::cout << " " << vertex->id;
        }
        std::cout << std::endl;
    }

    return 0;
}