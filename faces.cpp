#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Ponto{
    public:
        double x, y;

        Ponto(){
            this->x = 0;
            this->y = 0;
        }

        Ponto(double _x, double _y){
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

        std::vector<Aresta*> lista_arestas;

        Vertice(){
            this->id = 0;
            this->cord = Ponto();
        }

        Vertice(int _id, double _x, double _y){
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

double inclinacaoRelativa(const Ponto& p, const Ponto& q) {
    return atan2(q.y - p.y, q.x - p.x);
}

bool compararInclinacao(const Ponto& a, const Ponto& b, const Ponto& A, const Ponto& B) {
    double anguloA = inclinacaoRelativa(B, A);
    double angulo1 = inclinacaoRelativa(B, a) - anguloA;
    double angulo2 = inclinacaoRelativa(B, b) - anguloA;

    if (angulo1 < 0) angulo1 += 2 * M_PI;
    if (angulo2 < 0) angulo2 += 2 * M_PI;

    return angulo1 < angulo2;
}

void sortAntiHorario(Vertice* A, Vertice* B) {
    std::vector<std::pair<Ponto, Aresta*>> pontosEArestdas;

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        pontosEArestdas.push_back(std::make_pair(B->lista_arestas[i]->v2->cord, B->lista_arestas[i]));
    }

    std::sort(pontosEArestdas.begin(), pontosEArestdas.end(), [&](const std::pair<Ponto, Aresta*>& a, const std::pair<Ponto, Aresta*>& b) {
        return compararInclinacao(a.first, b.first, A->cord, B->cord);
    });

    for (int i = 0; i < B->lista_arestas.size(); i++) {
        B->lista_arestas[i] = pontosEArestdas[i].second;
    }
}

int main(){

    std::vector<Vertice*> vertices;
    std::vector<std::vector<int>> listasAdjacencia;

    int n, m;
    std::cin >> n >> m;
    listasAdjacencia.resize(n);
    for(int i = 0; i < n; i++){
        double x, y;
        int grau;
        std::cin >> x >> y >> grau;
        vertices.push_back(new Vertice(i + 1, x, y));

        for(int j = 0; j < grau; j++){
            int adjacente;
            std::cin >> adjacente;
            listasAdjacencia[i].push_back(adjacente - 1);
        }
    }

    std::vector<Aresta*> arestas;

    for(int i = 0; i < vertices.size(); i++){
        for(int j = 0; j < listasAdjacencia[i].size(); j++){
            Aresta* nova = (new Aresta(vertices[i], vertices[listasAdjacencia[i][j]]));
            arestas.push_back(nova);
            vertices[i]->lista_arestas.push_back(nova);
        }
    }


    std::vector<std::vector<Vertice*>> faces;

    Aresta* inicial = arestas[0];

    while (inicial->visitada != true) {
        Aresta* atual = inicial;
        std::vector<Vertice*> face;

        face.push_back(inicial->v1);
        while (atual->visitada != true) {
            face.push_back(atual->v2);
            atual->visitada = true;

            sortAntiHorario(atual->v1, atual->v2);

            for (int j = 0; j < atual->v2->lista_arestas.size(); j++) {
                if (atual->v2->lista_arestas[j]->v2 != atual->v1) {
                    atual = atual->v2->lista_arestas[j];
                    break;
                }
                else if (atual->v2->lista_arestas.size() < 2) {
                    atual = atual->v2->lista_arestas[j];
                    break;
                }
            }
        }

        faces.push_back(face);

        for (Aresta* a : arestas) {
            if (!a->visitada) {
                inicial = a;
                break;
            }
        }
    }

    if(faces.size() > 0){
        std::cout << faces.size() << '\n';
        for (int i = 0; i < faces.size(); i++) {
            if(faces[i].size() > 0 ){
                std::cout << faces[i].size() << ' ';
                for (int j = 0; j < faces[i].size(); j++) {
                    std::cout << faces[i][j]->id << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    for(int i = 0; i < arestas.size(); i++){
        delete arestas[i];
    }

    for(int i = 0; i < vertices.size(); i++){
        delete vertices[i];
    }

    return 0;
}
