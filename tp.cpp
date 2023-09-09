#include <iostream>
#include <vector>

class Vertice{
    public:
        int numero;
        int x;
        int y;
        int grau;
        std::vector<int> lista_adj;

        Vertice(){
            this->numero = 0;
            this->x = 0;
            this->y = 0;
            this->grau = 0;
        }
};

int main(){
    std::cout << "Insira n(número de vértices) e m(número de arestas)\n";
    int n, m;
    std::cin >> n >> m;

    std::vector<Vertice> vertices;
    int i = 0;
    for(int i = 0; i < n; i++){
        Vertice v;
        v.numero = i + 1;
        std::cin >> v.x >> v.y >> v.grau;

        for(int j = 0; j < v.grau; j++){
            int w;
            std::cin >> w;
            v.lista_adj.push_back(w);
        }

        vertices.push_back(v);
    }

    for(int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i].numero << '\n' << vertices[i].x << ' ' << vertices[i].y << ' ' << vertices[i].grau;

        for(int j = 0; j < vertices[i].grau; j++){
            std::cout << ' ' << vertices[i].lista_adj[j];
        }

        std::cout << '\n';
    }



    return 0;
}