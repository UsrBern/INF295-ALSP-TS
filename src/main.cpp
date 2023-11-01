#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "../include/greedy.hpp"


int main() {
    std::ifstream inputFile("../instances/airland1.txt");
    if (!inputFile) {
        std::cerr << "Error: Failed to open instance file." << std::endl;
        return 1;
    }

    int p;
    inputFile >> p;

    std::vector<int> E(p), T(p), L(p), g(p), h(p);
    std::vector<std::vector<int>> S(p, std::vector<int>(p));

    for (int i = 0; i < p; i++) {
        inputFile >> E[i] >> T[i] >> L[i] >> g[i] >> h[i];
        for (int j = 0; j < p; j++) {
            inputFile >> S[i][j];
        }
    }

    inputFile.close();

    std::vector<int> solution(p, -1);

    greedyAlgorithm(solution, E, T, L, g, h, S);

    for (int i = 0; i < p; i++) {
        std::cout << "Plane " << i + 1 << ": Landing Time = " << solution[i] << std::endl;
    }

    int totalPenalization = 0;
    for (int i = 0; i < p; i++) {
        totalPenalization += calculatePenalization(solution[i], T[i], g[i], h[i]);
    }
    std::cout << "Total Penalization: " << totalPenalization << std::endl;

    return 0;
}
