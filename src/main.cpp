#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include "../include/greedy.hpp"

#include <algorithm>

void printSolution(const std::vector<Plane>& planes, int totalPenalization, double computationTime) {
    // Sort the planes vector by their landing time
    std::vector<Plane> sortedPlanes = planes;
    std::sort(sortedPlanes.begin(), sortedPlanes.end(), [](const Plane& a, const Plane& b) {
        return a.assignedLandingTime < b.assignedLandingTime;
    });

    std::cout << "| Plane |";
    for (const Plane& plane : sortedPlanes) {
        std::cout << " " << std::setw(6) << plane.planeID << " |";
    }
    std::cout << "\n| Time  |";
    for (const Plane& plane : sortedPlanes) {
        std::cout << " " << std::setw(6) << plane.assignedLandingTime << " |";
    }
    std::cout << std::endl;
    std::cout << std::endl;

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

    // Close input file
    inputFile.close();

    // Create a Runway object
    Runway runway(planes);

    // Create a solution vector and compute the initial solution
    std::vector<int> solution(p, -1);
    auto start = std::chrono::high_resolution_clock::now();
    int totalPenalization = greedyAlgorithm(runway);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = end - start;

    // Print solution
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Total Cost: " << totalPenalization << std::endl;
    std::cout << "Computation Time: " << computationTime << " ms" << std::endl;

    return 0;
}
