#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stack>

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

        Vertice(int _id, long double _x, long double _y, int _grau){
            this->id = _id;
            this->cord = Ponto(_x, _y);
            this->grau = _grau;
            this->cor = "branco";
        }

        Vertice(int _id, long double _x, long double _y, int _grau, std::vector<Vertice*> _lista_adj){
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

bool compareByPolarAngle(const Vertice* a, const Vertice* b, const Vertice* reference) {
    long double angle_a = atan2l(a->cord.y - reference->cord.y, a->cord.x - reference->cord.x);
    long double angle_b = atan2l(b->cord.y - reference->cord.y, b->cord.x - reference->cord.x);
    return angle_a < angle_b;
}

void sortAdjacentVerticesAndEdgesByPolarAngle(Vertice* reference, Vertice* ordered) {
    std::vector<std::pair<Vertice*, Aresta*>> adjAndEdges;

    for (int i = 0; i < ordered->lista_adj.size(); ++i) {
        Vertice* adj = ordered->lista_adj[i];
        Aresta* edge = ordered->lista_arestas[i];

        adjAndEdges.push_back(std::make_pair(adj, edge));
    }

    std::sort(adjAndEdges.begin(), adjAndEdges.end(),
              [reference](const std::pair<Vertice*, Aresta*>& a, const std::pair<Vertice*, Aresta*>& b) {
                  return compareByPolarAngle(a.first, b.first, reference);
              });

    for (int i = 0; i < adjAndEdges.size(); ++i) {
        ordered->lista_adj[i] = adjAndEdges[i].first;
        ordered->lista_arestas[i] = adjAndEdges[i].second;
    }
}

void acharFacesDFS(Aresta* aresta, Aresta* arestaInicial, std::vector<Vertice*> &face){
    sortAdjacentVerticesAndEdgesByPolarAngle(aresta->v1, aresta->v2);

    if(aresta->visitas > 0){
        return;
    }

    face.push_back(aresta->v1);
    aresta->visitas++;

    for(int i = 0; i < aresta->v2->lista_arestas.size(); i++){
        sortAdjacentVerticesAndEdgesByPolarAngle(aresta->v2, aresta->v2->lista_adj[i]);

        if(aresta->v2->lista_arestas[i]->v1 == arestaInicial->v1 && aresta->v2->lista_arestas[i]->v2 == arestaInicial->v2){
            face.push_back(aresta->v2->lista_arestas[i]->v1);
            return;
        }
        else if((aresta->v2->lista_arestas[i]->visitas == 0) && ((aresta->v2->lista_adj[i] != aresta->v1))){
            acharFacesDFS(aresta->v2->lista_arestas[i], arestaInicial, face);
            return;
        }
    }
    return;
}

void acharFaces(std::vector<Aresta*> arestas, std::vector<std::vector<Vertice*>> &faces){
    for(int i = 0; i < arestas.size(); i++){
        if(arestas[i]->visitas == 0){
            std::vector<Vertice*> face;

            Aresta* arestaInicial = new Aresta(arestas[i]->v1, arestas[i]->v2);
            acharFacesDFS(arestas[i], arestaInicial, face);
            delete arestaInicial;
            
            faces.push_back(face);
        }
    }
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
        vertices.push_back(new Vertice(i, x, y, grau));

        for(int j = 0; j < grau; j++){
            int adjacente;
            std::cin >> adjacente;
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    }
    
    for (int i = 0; i < vertices.size(); i++){
        for (int j = 0; j < listasAdjacencia[i].size(); j++){
            vertices[i]->lista_adj.push_back(vertices[listasAdjacencia[i][j]]);
            vertices[i]->lista_arestas.push_back(new Aresta(vertices[i], vertices[listasAdjacencia[i][j]]));
        }
    }

    std::vector<Aresta*> arestas;
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < vertices[i]->lista_adj.size(); j++) {
            if (vertices[i]->lista_arestas.size() > j) {
                arestas.push_back(vertices[i]->lista_arestas[j]);
            }
        }
    }

    std::vector<std::vector<Vertice*>> faces;
    acharFaces(arestas, faces);

    std::cout << faces.size() << '\n';
    for(int i = 0; i < faces.size(); i++){
        for(int j = 0; j < faces[i].size(); j++){
            std::cout << faces[i].size() << ' ' << faces[i][j]->id + 1 << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}