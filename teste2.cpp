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
        bool rabo = false;

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

bool compareByRelativePosition(const Vertice* a, const Vertice* b, const Vertice* reference) {
    // Calculate the cross product of vectors (reference -> a) and (reference -> b)
    long double crossProduct = (a->cord.x - reference->cord.x) * (b->cord.y - reference->cord.y) -
                               (b->cord.x - reference->cord.x) * (a->cord.y - reference->cord.y);

    if (crossProduct > 0) {
        return true;  // 'a' is clockwise from 'b' with respect to the reference point
    } else if (crossProduct < 0) {
        return false;  // 'a' is counterclockwise from 'b' with respect to the reference point
    } else {
        // If the cross product is zero, compare by distance (closer points first)
        long double distanceA = (a->cord.x - reference->cord.x) * (a->cord.x - reference->cord.x) +
                               (a->cord.y - reference->cord.y) * (a->cord.y - reference->cord.y);
        long double distanceB = (b->cord.x - reference->cord.x) * (b->cord.x - reference->cord.x) +
                               (b->cord.y - reference->cord.y) * (b->cord.y - reference->cord.y);

        return distanceA < distanceB;
    }
}

void sortAdjacentVerticesAndEdgesByRelativePosition(Vertice* reference, Vertice* ordered) {
    std::vector<std::pair<Vertice*, Aresta*>> adjAndEdges;

    for (int i = 0; i < ordered->lista_adj.size(); ++i) {
        Vertice* adj = ordered->lista_adj[i];
        Aresta* edge = ordered->lista_arestas[i];

        adjAndEdges.push_back(std::make_pair(adj, edge));
    }

    std::sort(adjAndEdges.begin(), adjAndEdges.end(),
              [reference](const std::pair<Vertice*, Aresta*>& a, const std::pair<Vertice*, Aresta*>& b) {
                  return compareByRelativePosition(a.first, b.first, reference);
              });

    for (int i = 0; i < adjAndEdges.size(); ++i) {
        ordered->lista_adj[i] = adjAndEdges[i].first;
        ordered->lista_arestas[i] = adjAndEdges[i].second;
    }
}

long double Inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}

// Coeficiente da reta orientada de p para q.
long double InclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}

void sortAdjacentVerticesAndEdgesByPolarAngle(Vertice* reference, Vertice* ordered) {
    for(int k = 0; k < ordered->lista_adj.size() - 1; k++){
        for(int i = 0; i < ordered->lista_adj.size() - 1; i++){
            if(InclinacaoRelativa(reference->cord, ordered->lista_adj[i]->cord) > InclinacaoRelativa(reference->cord, ordered->lista_adj[i + 1]->cord)){
                Vertice* temp = ordered->lista_adj[i];
                ordered->lista_adj[i] = ordered->lista_adj[i + 1];
                ordered->lista_adj[i + 1] = temp;
            }
        }
    }

    for(int i = 0; i < ordered->lista_adj.size(); i++){
        for(int j = 0; j < ordered->lista_adj.size(); j++){
            if(ordered->lista_arestas[j]->v2 == ordered->lista_adj[i]){
                auto temp = ordered->lista_arestas[j];
                ordered->lista_arestas[j] = ordered->lista_arestas[i];
                ordered->lista_arestas[i] = temp;
            }
        }
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


void acharFacesDFS(Aresta* aresta, Aresta* arestaInicial, std::vector<Vertice*> &face, bool &rabo){
    sortAdjacentVerticesAndEdgesByRelativePosition(aresta->v1, aresta->v2);

    std::cout << "RABO: " << rabo << '\n';

    if(aresta->visitas > 0){
        return;
    }

    face.push_back(aresta->v1);
    std::cout << "adicionando " << aresta->v1->id << " pela aresta: " << aresta->v1->id << ',' << aresta->v2->id << '\n';
    aresta->visitas++;

    if(aresta->v1->lista_adj.size() < 2){
        return;
    }

    for(int i = 0; i < aresta->v2->lista_arestas.size(); i++){
        sortAdjacentVerticesAndEdgesByRelativePosition(aresta->v2, aresta->v2->lista_adj[i]);
        std::cout << "Ordenando " << aresta->v2->lista_adj[i]->id << ":\n";
        for(int j = 0; j < aresta->v2->lista_adj[i]->lista_adj.size(); j++){
            std::cout << aresta->v2->lista_adj[i]->lista_adj[j]->id << ' ';
        }
        std::cout << "\n";

        //Checa se chegou na aresta inicial
        if(aresta->v2->lista_arestas[i]->v1 == arestaInicial->v1 && aresta->v2->lista_arestas[i]->v2 == arestaInicial->v2){
            face.push_back(aresta->v2->lista_arestas[i]->v1);
            aresta->v2->lista_arestas[i]->visitas++;
            return;
        }
        //Checa se chegou no final de um rabo
        if((aresta->v2->lista_arestas[i]->v2 == aresta->v1) && ((aresta->v2->lista_adj.size() < 2))){
            face.push_back(aresta->v2->lista_arestas[i]->v1);
            aresta->v2->lista_arestas[i]->visitas++;
            rabo = true;

            aresta->v2->lista_arestas[i]->v1->rabo = true;
            acharFacesDFS(aresta->v2->lista_arestas[i], arestaInicial, face, rabo);
            return;
        }
        //Avança na DFS normalmente
        if((aresta->v2->lista_arestas[i]->visitas == 0) && ((aresta->v2->lista_adj[i] != aresta->v1))){
            rabo = false;
            acharFacesDFS(aresta->v2->lista_arestas[i], arestaInicial, face, rabo);
            return;
        }
    }
    return;
}

void acharFaces(std::vector<Aresta*> arestas, std::vector<std::vector<Vertice*>> &faces){
    bool rabo = false;
    for(int i = 0; i < arestas.size(); i++){
        if(arestas[i]->visitas == 0){
            std::vector<Vertice*> face;

            Aresta* arestaInicial = new Aresta(arestas[i]->v1, arestas[i]->v2);
            acharFacesDFS(arestas[i], arestaInicial, face, rabo);
            delete arestaInicial;

            faces.push_back(face);
        }
    }
}

int main(){
    std::vector<Vertice*> vertices = {new Vertice(0, 0, 0, 2), new Vertice(1, 1, 0, 2), new Vertice(2, 1, 1, 2), new Vertice(3, 0, 1, 2), new Vertice(4, 1.5, 0.5, 2), new Vertice(5, -0.5, -0.5, 2), new Vertice(6, -1, -1, 2), new Vertice(7, -2, -2, 1)};
    vertices[0]->lista_adj = {vertices[1], vertices[3], vertices[5]};
    vertices[0]->lista_arestas.push_back(new Aresta(vertices[0], vertices[1]));
    vertices[0]->lista_arestas.push_back(new Aresta(vertices[0], vertices[3]));
    vertices[0]->lista_arestas.push_back(new Aresta(vertices[0], vertices[5]));

    vertices[1]->lista_adj = {vertices[0], vertices[2], vertices[3], vertices[4]};
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[0]));
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[2]));
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[3]));
    vertices[1]->lista_arestas.push_back(new Aresta(vertices[1], vertices[4]));

    vertices[2]->lista_adj = {vertices[1], vertices[3], vertices[4]};
    vertices[2]->lista_arestas.push_back(new Aresta(vertices[2], vertices[1]));
    vertices[2]->lista_arestas.push_back(new Aresta(vertices[2], vertices[3]));
    vertices[2]->lista_arestas.push_back(new Aresta(vertices[2], vertices[4]));

    vertices[3]->lista_adj = {vertices[2], vertices[0], vertices[1]};
    vertices[3]->lista_arestas.push_back(new Aresta(vertices[3], vertices[2]));
    vertices[3]->lista_arestas.push_back(new Aresta(vertices[3], vertices[0]));
    vertices[3]->lista_arestas.push_back(new Aresta(vertices[3], vertices[1]));

    vertices[4]->lista_adj = {vertices[1], vertices[2]};
    vertices[4]->lista_arestas.push_back(new Aresta(vertices[4], vertices[1]));
    vertices[4]->lista_arestas.push_back(new Aresta(vertices[4], vertices[2]));

    vertices[5]->lista_adj = {vertices[0], vertices[6]};
    vertices[5]->lista_arestas.push_back(new Aresta(vertices[5], vertices[0]));
    vertices[5]->lista_arestas.push_back(new Aresta(vertices[5], vertices[6]));

    vertices[6]->lista_adj = {vertices[5], vertices[7]};
    vertices[6]->lista_arestas.push_back(new Aresta(vertices[6], vertices[5]));
    vertices[6]->lista_arestas.push_back(new Aresta(vertices[6], vertices[7]));

    vertices[7]->lista_adj = {vertices[6]};
    vertices[7]->lista_arestas.push_back(new Aresta(vertices[7], vertices[6]));

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