#include <iostream>
#include <vector>
#include <cmath>

struct Ponto{
    double x, y;
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

class Vertice{
    public:
        int numero;
        Ponto cord;
        int grau;
        std::vector<int> lista_adj;

        Vertice(){
            this->numero = 0;
            this->cord.x = 0;
            this->cord.y = 0;
            this->grau = 0;
        }
};

std::vector<Vertice> vertices;

void swap(Vertice* xp, Vertice* yp){
    Vertice temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
// An optimized version of Bubble Sort
void bubblePolarSort(std::vector<Vertice> lista_adj){
    int i, j;
    bool swapped;
    for (i = 0; i < lista_adj.size() - 1; i++) {
        swapped = false;
        for (j = 0; j < lista_adj.size() - i - 1; j++) {
            if (atan2(lista_adj[j].cord.x, lista_adj[j].cord.y) > atan2(lista_adj[j+1].cord.x, lista_adj[j+1].cord.y)) {
                swap(&lista_adj[j], &lista_adj[j + 1]);
                swapped = true;
            }
        }
  
        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
}



int main(){
    std::cout << "Insira n(número de vértices) e m(número de arestas)\n";
    int n, m;
    std::cin >> n >> m;

    int i = 0;
    for(int i = 0; i < n; i++){
        Vertice v;
        v.numero = i;
        std::cin >> v.cord.x >> v.cord.y >> v.grau;

        for(int j = 0; j < v.grau; j++){
            int w;
            std::cin >> w;
            v.lista_adj.push_back(w);
        }

        vertices.push_back(v);
    }

    for(int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i].numero << '\n' << vertices[i].cord.x << ' ' << vertices[i].cord.y << ' ' << vertices[i].grau;

        for(int j = 0; j < vertices[i].grau; j++){
            std::cout << ' ' << vertices[i].lista_adj[j];
        }

        std::cout << '\n';
    }



    return 0;
}