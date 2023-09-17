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

        Aresta* aresta;
        std::vector<Vertice*> lista_adj;
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
        int visitas;

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

void sortAdjacentVerticesByPolarAngle(Vertice* reference, Vertice* ordered) {
    std::sort(ordered->lista_adj.begin(), ordered->lista_adj.end(),
              [reference](const Vertice* a, const Vertice* b) {
                  return compareByPolarAngle(a, b, reference);
              });

    // Reverse the order to make it counterclockwise
    std::reverse(ordered->lista_adj.begin(), ordered->lista_adj.end());
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

void DFS2(Vertice* raiz){
    if(raiz->lista_adj[0]);
}

std::vector<std::vector<Vertice*>> acharFaces(std::vector<Vertice*> vertices){
    std::vector<std::vector<Vertice*>> faces;
    faces.resize(20);
    int f = 0;
    for(int i = 0; i < vertices.size(); i++){
        //faces.resize(f++);
        faces[i].push_back(vertices[i]);
        vertices[i]->aresta->visitas++;

        std::cout << vertices[i]->id;
        if(vertices[i]->lista_adj.size() > 0){
            sortAdjacentVerticesByPolarAngle(vertices[i], vertices[i]->lista_adj[0]);
            if(vertices[i]->lista_adj[0]->aresta->visitas > 0){
                break;
            }

            //faces.resize(f++);
            faces[i].push_back(vertices[i]->lista_adj[0]);
            vertices[i]->lista_adj[0]->aresta->visitas++;
        }

        for(int j = 0; j < vertices[i]->lista_adj.size(); j++){
            if(vertices[i]->lista_adj[j]->lista_adj.size() > 0){
                sortAdjacentVerticesByPolarAngle(vertices[i]->lista_adj[j], vertices[i]->lista_adj[j]->lista_adj[0]);
                if(vertices[i]->lista_adj[j]->lista_adj[0]->aresta->visitas > 0){
                    break;
                }

                //faces.resize(f++);
                faces[i].push_back(vertices[i]->lista_adj[j]->lista_adj[0]);
                vertices[i]->lista_adj[j]->lista_adj[0]->aresta->visitas++;
            }
        }
    }
    return faces;
}

int main(){
    std::vector<Vertice*> vertices = {new Vertice(0, 0, 0, 2), new Vertice(1, 1, 0, 2), new Vertice(2, 1, 1, 2), new Vertice(3, 0, 1, 2)};
    vertices[0]->lista_adj = {vertices[1], vertices[3]};
    vertices[1]->lista_adj = {vertices[0], vertices[2]};
    vertices[2]->lista_adj = {vertices[1], vertices[3]};
    vertices[3]->lista_adj = {vertices[2], vertices[0]};

    vertices.push_back(new Vertice(4, 0, 5, 5));

    std::vector<Aresta*> arestas;
    for(int i = 0; i < vertices.size(); i++){
        for(int j = 0; j < vertices[i]->lista_adj.size(); j++){
            vertices[i]->aresta = new Aresta(vertices[i], vertices[i]->lista_adj[j]);
        }
    }

    std::cout << vertices[0]->aresta->v1->id;

    /*//DFS PADRÃO
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->cor == "preto"){
            i++;
        }
        else{
            std::cout << "DFS\n";
            DFS(vertices[i]);
        }
    }*/

    std::vector<std::vector<Vertice*>> faces = acharFaces(vertices);

    /*for(int i = 0; i < faces.size(); i++){
        for(int j = 0; j < faces[i].size(); j++){
            std::cout << faces[i][j]->id << ' ';
        }
        std::cout << '\n';
    }*/

    return 0;
}