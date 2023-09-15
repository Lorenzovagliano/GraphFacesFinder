#include <iostream>
#include <cmath>
#include <vector>

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

class Vertice{
    public:
        int id;
        Ponto cord;
        int grau;
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

//Ordena o 1o elemento da lista de adjacência de um vértice em relação a outro com base na inclinação relativa entre os dois
void ordenarRelativo(Vertice* vertice, Vertice* verticeAntigo){
    if(vertice->lista_adj.size() < 2){
        return;
    }

    Vertice* menor;
    for(int i = 0; i < vertice->lista_adj.size() - 1; i++){
        if(inclinacaoRelativa(verticeAntigo->cord, vertice->lista_adj[i]->cord) < inclinacaoRelativa(verticeAntigo->cord, vertice->lista_adj[i + 1]->cord)){
            Vertice* menor = vertice->lista_adj[i];
        }
    }
    Vertice* temp = vertice->lista_adj[0];
    vertice->lista_adj[0] = menor;
    vertice->lista_adj[vertice->lista_adj.size()] = vertice->lista_adj[0];
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

int main(){
    std::vector<Vertice*> vertices = {new Vertice(0, 4, 0, 0), new Vertice(1, 2, 1, 1), new Vertice(2, 1, -1, -1), new Vertice(3, 2, 1, -1), new Vertice(4, 1, -1, 1)};
    vertices[0]->lista_adj = {vertices[1], vertices[2], vertices[3], vertices[4]};

    vertices[1]->lista_adj.push_back(vertices[0]);
    vertices[1]->lista_adj.push_back(vertices[3]);
    vertices[3]->lista_adj.push_back(vertices[1]);
    vertices[3]->lista_adj.push_back(vertices[0]);

    vertices.push_back(new Vertice(5, 0, 5, 5));

    ordenarRelativo(vertices[0]->lista_adj[0], vertices[0]);

    std::cout << vertices[0]->lista_adj[0]->id << '\n';

    //DFS PADRÃO
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->cor == "preto"){
            i++;
        }
        else{
            std::cout << "DFS\n";
            DFS(vertices[i]);
        }
    }

    return 0;
}