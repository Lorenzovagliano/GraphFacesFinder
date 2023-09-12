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



int main(){
    std::vector<int> nums = {5, 4, 3, 2, 1, 0};
    ordenar(nums);

    for(int i = 0; i < nums.size(); i++){
        std::cout << nums[i] << ' ';
    }

    return 0;
}