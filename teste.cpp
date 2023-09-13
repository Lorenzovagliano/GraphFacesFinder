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

// Determina se ao caminhar de a para b e depois de b para c estamos fazendo uma curva `a esquerda, `a direita, ou seguindo em frente.
int TipoCurva(Ponto a, Ponto b, Ponto c) {
    double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v < 0) return -1; // esquerda.
    if (v > 0) return +1; // direita.
    return 0; // em frente.
}

//Ordena os vértices adjacentes a certo vértice de maneira anti-horária, baseando-se no angulo polar.
void ordenarPolar(Vertice* vertice){
    if(vertice->lista_adj.size() < 1){
        return;
    }
    for(int k = 0; k < vertice->lista_adj.size() - 1; k++){
        for(int i = 0; i < vertice->lista_adj.size() - 1; i++){
            if(inclinacaoRelativa(vertice->cord, vertice->lista_adj[i]->cord) > inclinacaoRelativa(vertice->cord, vertice->lista_adj[i + 1]->cord)){
                Vertice* temp = vertice->lista_adj[i];
                vertice->lista_adj[i] = vertice->lista_adj[i+1];
                vertice->lista_adj[i+1] = temp;
            }
        }
    }
}

//Ordena todas as listas de adjacência de vértices conexos a certo vértice
void ordenarPolarRecursivo(Vertice* vertice){
    ordenarPolar(vertice);
    for(int i = 0; i < vertice->lista_adj.size(); i++){
        if(vertice->lista_adj.size() < 1){
            return;
        }
        ordenarPolarRecursivo(vertice->lista_adj[0]);
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


int main(){
    Vertice* v = new Vertice(0, 4, 0, 0);
    v->lista_adj = {new Vertice(1, 1, 1, 1), new Vertice(2, 1, -1, -1), new Vertice(3, 1, 1, -1), new Vertice(4, 1, -1, 1)};
    v->lista_adj[0]->lista_adj.push_back(v->lista_adj[2]);
    v->lista_adj[2]->lista_adj.push_back(v->lista_adj[0]);
    

    ordenarPolarRecursivo(v);

    for(int i = 0; i < v->lista_adj.size(); i++){
        std::cout << v->lista_adj[i]->id << ' ';
    }
    std::cout << '\n';

    DFS(v);

    for(int i = 0; i < v->lista_adj.size(); i++){
        std::cout << v->lista_adj[i]->cor << ' ';
    }
    std::cout << '\n';



    return 0;
}