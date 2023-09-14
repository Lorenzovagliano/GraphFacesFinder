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
    if(vertice->lista_adj.size() < 2){
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

void DFS2(Vertice* raiz, std::vector<Vertice*> &face, int id){
        raiz->cor = "cinza";
        std::cout << raiz->id << " Ficou cinza\n";
        for(int i = 0; i < raiz->lista_adj.size(); i++){
            if(raiz->lista_adj[i]->id == id){
                std::cout << "Adicionando " << raiz->lista_adj[i]->id << '\n';
                face.push_back(raiz->lista_adj[i]);
            }
            if(raiz->lista_adj[i]->cor == "branco"){
                DFS2(raiz->lista_adj[i], face, id);
            }
        }
        raiz->cor = "preto";
        std::cout << raiz->id << " Ficou preto\n";
}

int main(){
    std::vector<Vertice*> vertices = {new Vertice(0, 4, 0, 0), new Vertice(1, 2, 1, 1), new Vertice(2, 1, -1, -1), new Vertice(3, 2, 1, -1), new Vertice(4, 1, -1, 1)};
    vertices[0]->lista_adj = {vertices[1], vertices[2], vertices[3], vertices[4]};
    vertices[1]->lista_adj = {vertices[3]};
    vertices[3]->lista_adj = {vertices[1]};
    vertices[3]->lista_adj = {vertices[0]};

    vertices[2]->lista_adj = {vertices[4]};
    vertices[4]->lista_adj = {vertices[2]};
    vertices[4]->lista_adj = {vertices[0]};

    vertices.push_back(new Vertice(5, 0, 9, 9));
    
    //ordena o grafo inteiro
    for(int i = 0; i < vertices.size(); i++){
        ordenarPolar(vertices[i]);
    }

    //lista os adjacentes a vertices[0]
    for(int i = 0; i < vertices[0]->lista_adj.size(); i++){
        std::cout << vertices[0]->lista_adj[i]->id << ' ';
    }
    std::cout << '\n';

    //Faz a DFS no grafo inteiro e aramazena uma das faces
    std::vector<Vertice*> face;
    int k = 0;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i]->cor == "preto"){ //ta errado aqui
            std::cout << "Adicionando " << vertices[i]->id << '\n';
            face.push_back(vertices[i]);
            k = vertices[i]->lista_adj[0]->id;
            i++;
        }
        else{
            DFS2(vertices[i], face, vertices[i]->id);
        }
        vertices[i]->cor == "branco";
    }
    std::cout << "Adicionando " << vertices[k]->id << '\n';
    face.push_back(vertices[k]);

    //imprimir a face
    std::cout << '\n';
    for(int i = 0; i < face.size(); i++){
        std::cout << face[i]->id << ' ';
    }
    std::cout << '\n';

    return 0;
}