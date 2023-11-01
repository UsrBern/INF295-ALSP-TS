#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
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

    // Measure computation time
    auto start_time = std::chrono::high_resolution_clock::now();

    greedyAlgorithm(solution, E, T, L, g, h, S);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print the solution table
    std::cout << "______________________" << std::endl;
    std::cout << "| Plane |";
    for (int i = 0; i < p; i++) {
        std::cout << " " << i + 1 << " |";
    }
    std::cout << std::endl;

    std::cout << "| Time  |";
    for (int i = 0; i < p; i++) {
        std::cout << " " << solution[i] << " |";
    }
    std::cout << std::endl;
    std::cout << "--------------------------\n";

    int totalPenalization = 0;
    for (int i = 0; i < p; i++) {
        totalPenalization += calculatePenalization(solution[i], T[i], g[i], h[i]);
    }

    // Print the total penalization and computation time
    std::cout << "Total Cost: " << totalPenalization << std::endl;
    std::cout << "Computation Time: " << duration.count() << " ms" << std::endl;

    return 0;
}
