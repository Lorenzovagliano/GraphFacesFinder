#include <iostream>
#include <vector>
#include <cmath>

void ordenar(std::vector<int> &vetor){
    for(int k = 0; k < vetor.size() - 1; k++){
        for(int i = 0; i < vetor.size() - k; i++){
            if(vetor[i] > vetor[i + 1]){
                int temp = vetor[i];
                vetor[i] = vetor[i+1];
                vetor[i+1] = temp;
            }
        }
    }
}

    std::vector<std::vector<Vertice*>> faces;

    for(int k = 0; k < arestas.size(); k++){

        if(arestas[k]->visitada == true){
            continue;
        }

        Aresta* atual = arestas[k];
        std::vector<Vertice*> face;

        while (atual->visitada != true) {
            face.push_back(atual->v1);
            std::cout << "Adicionando " << atual->v1->id << '\n';
            atual->visitada = true;

            orderAdjacentEdgesByAngle(atual->v1, atual->v2);

            std::cout << atual->v2->id << ": ";
            for (int i = 0; i < atual->v2->lista_arestas.size(); i++) {
                std::cout << atual->v2->lista_arestas[i]->v2->id << ", ";
            }
            std::cout << '\n';

            // Find the next unvisited edge with the smallest polar angle
            for (int j = 0; j < atual->v2->lista_arestas.size(); j++) {
                if (!atual->v2->lista_arestas[j]->visitada) {
                    if(atual->v2->lista_arestas.size() < 2){
                        atual = atual->v2->lista_arestas[j];
                        break;
                    }
                    if(atual->v2->lista_arestas.size() >= j + 2){
                        if(atual->v2->lista_arestas[j+1]->visitada == true){
                            break;
                        }
                        atual = atual->v2->lista_arestas[j+1];
                        break;
                    }
                }
            }
        }
        faces.push_back(face);

    }



int main(){
    std::vector<int> nums = {5, 4, 3, 2, 1, 0};
    ordenar(nums);

    for(int i = 0; i < nums.size(); i++){
        std::cout << nums[i] << ' ';
    }

    return 0;
}