#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <random>
#include <regex>
#include "headers/greedy.hpp"
#include "headers/tabu.hpp"

#include <algorithm>

int main() {
    // Read input file
    std::string fileName;
    std::regex fileNamePattern("airland([1-9]|1[0-3])\\.txt$");
    std::ifstream inputFile;

    do {
        std::cout << "Enter the name of the input file (ex: airland1.txt): ";
        std::cin >> fileName;

        if (!std::regex_match(fileName, fileNamePattern)) {
            std::cout << "Error: Invalid input file name. Try again." << std::endl;
        } else {
            inputFile.open("src/instances/" + fileName);
            if (!inputFile.is_open()) {
                std::cout << "Error: Could not open file. Try again." << std::endl;
            }
        }
    } while (!inputFile.is_open());

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

        planes.push_back(Plane(E, T, L, g, h, S));
    }

    // Close input file
    inputFile.close();

    // Create a Runway object
    Runway runway(planes);
    char printAsk;
    std::cout << "Print the runway parameters? (y/n) ";
    std::cin >> printAsk;
    if (printAsk == 'y') {
        runway.print();
    }
    
    // Create a solution vector and compute the initial solution
    std::cout << "Computing initial solution..." << std::endl;
    greedyAlgorithm(runway, p);
    runway.printRunwaySchedule();

    // Initialize Tabu-Search
    // Randomize the tabu tenure within a range [minTabuTenure, maxTabuTenure]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(p*0.4, p*0.8);
    size_t tabuTenure = dis(gen);
    TabuSearch tabu(tabuTenure);

    // Run Tabu-Search
    std::cout << "Running Tabu-Search..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int noImprovementLimit = 100;  // Stop if no improvement after 100 iterations
    int iterationsWithoutImprovement = 0;
    int bestPenalization = INT_MAX;

    while (iterationsWithoutImprovement < noImprovementLimit) {
        tabu.search(runway, p, 5);
        int currentPenalization = evaluationFunction(runway, p);
        if (currentPenalization < bestPenalization) {
            bestPenalization = currentPenalization;
            iterationsWithoutImprovement = 0;
        } else {
            iterationsWithoutImprovement++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = end - start;

    int totalPenalization = evaluationFunction(runway, p);

    // Print solution
    runway.printRunwaySchedule();
    std::cout << std::endl;

    std::cout << "Costo Total: " << totalPenalization << std::endl;
    std::cout << "Tiempo de Computo: " << elapsedTime.count() << " [seg]" << std::endl;

    return 0;
}
