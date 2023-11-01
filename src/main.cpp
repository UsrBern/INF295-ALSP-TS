#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../include/greedy.hpp"

void printSolution(const std::vector<Plane>& planes, int totalPenalization, double computationTime) {
    std::cout << "______________________" << std::endl;
    std::cout << "| Plane |";
    for (const Plane& plane : planes) {
        std::cout << " " << std::setw(6) << plane.planeID << " |";
    }
    std::cout << "\n| Time  |";
    for (const Plane& plane : planes) {
        std::cout << " " << std::setw(6) << plane.assignedLandingTime << " |";
    }
    std::cout << "\n----------------------" << std::endl;

    std::cout << "Total Cost: " << totalPenalization << std::endl;
    std::cout << "Computation Time: " << computationTime << " ms" << std::endl;
}


int main() {
    // Read input file
    std::ifstream inputFile("../instances/airland1.txt");
    if (!inputFile.is_open()) {
        std::cout << "Error: could not open input file" << std::endl;
        return 1;
    }

    // Read number of planes
    int p;
    inputFile >> p;

    // Read planes
    std::vector<Plane> planes;
    for (int i = 0; i < p; i++) {
        int E, T, L;
        float g, h;
        inputFile >> E >> T >> L >> g >> h;

        std::vector<int> S;
        for (int j = 0; j < p; j++) {
            int s;
            inputFile >> s;
            S.push_back(s);
        }        

        planes.push_back(Plane(E, T, L, g, h, S, i+1));
    }
    for (size_t i = 0; i < planes.size(); i++) {
        planes[i].print();
    }

    // Close input file
    inputFile.close();

    // Create a solution vector and compute the initial solution
    std::vector<int> solution(p, -1);
    auto start = std::chrono::high_resolution_clock::now();
    greedyAlgorithm(planes);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = end - start;

    // Print solution
    printSolution(planes, , elapsedTime.count() * 1000);

    return 0;
}
