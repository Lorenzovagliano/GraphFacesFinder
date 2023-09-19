#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

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

        std::vector<Vertice*> lista_adj;
        std::vector<Aresta*> lista_arestas;

        Vertice(){
            this->id = 0;
            this->cord = Ponto();
        }

        Vertice(int _id, long double _x, long double _y, int _grau){
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

long double inclinacaoRelativa(Ponto p, Ponto q) {
    return atan2l(q.y - p.y, q.x - p.x);
}

std::vector<Vertice*> ordenarPolar(Vertice* referencia, Vertice* ordenado){
    for(int k = 0; k < ordenado->lista_adj.size() - 1; k++){
        for(int i = 0; i < ordenado->lista_adj.size() - 1; i++){
            if(inclinacaoRelativa(referencia->cord, ordenado->lista_adj[i]->cord) > inclinacaoRelativa(referencia->cord, ordenado->lista_adj[i + 1]->cord)){
                Vertice* temporario = ordenado->lista_adj[i];
                ordenado->lista_adj[i] = ordenado->lista_adj[i+1];
                ordenado->lista_adj[i+1] = temporario;

                Aresta* temporario2 = ordenado->lista_arestas[i];
                ordenado->lista_arestas[i] = ordenado->lista_arestas[i+1];
                ordenado->lista_arestas[i+1] = temporario2;
            }
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
        vertices.push_back(new Vertice(i + 1, x, y, grau));

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


    std::vector<std::vector<Vertice*>> faces;

    for(int i = 0; i < arestas.size(); i++){
        Aresta* atual = arestas[i];

        std::vector<Vertice*> face;

        while(atual->visitada != true){
            face.push_back(atual->v1);
            std::cout << "Adicionando " << atual->v1->id << ", \n";

            for(int j = 0; j < atual->v2->lista_arestas.size(); j++){
                std:: cout << atual->v2->lista_arestas[j]->v1->id << ", " << atual->v2->lista_arestas[j]->v2->id << '\n';
            }
            
            ordenarPolar(atual->v1, atual->v2);

            face.push_back(atual->v2);
            std::cout << "Adicionando " << atual->v2->id << ", \n";

            for(int j = 0; j < atual->v2->lista_arestas.size(); j++){
                std:: cout << atual->v2->lista_arestas[j]->v1->id << ", " << atual->v2->lista_arestas[j]->v2->id << '\n';
            }

            atual->visitada = true;

            for(int j = 0; j < atual->v2->lista_arestas.size(); j++){
                if(inclinacaoRelativa(atual->v2->cord, atual->v2->lista_arestas[j]->v1->cord) > inclinacaoRelativa(atual->v1->cord, atual->v2->cord)){
                    atual = atual->v2->lista_arestas[j];
                }
            }
        }
        faces.push_back(face);
    }

    std::cout << faces.size() << '\n';
    for(int i = 0; i < faces.size(); i++){
        for(int j = 0; j < faces[i].size(); j++){
            std::cout << faces[i].size() << ' ' << faces[i][j]->id << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
