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


// Funcion main
// Pide el nombre de uno de los archivos airlandX.txt sobre el cual efectuar Greedy + Tabu-Search
// Imprime los resultados por pantalla
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

    // Create a solution vector and compute the initial solution
    std::cout << "Computing initial solution..." << std::endl;
    greedyAlgorithm(runway, p);
    Runway initialRunway = runway;
    std::cout << "Costo Total: " << evaluationFunction(runway, p) << std::endl;

    // Initialize Tabu-Search
    // Randomize the tabu tenure within a range [minTabuTenure, maxTabuTenure]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(p*0.4, p*0.6);
    size_t tabuTenure = p/10;
    TabuSearch tabu(tabuTenure);

    // Run Tabu-Search
    std::cout << "Running Tabu-Search..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int noImprovementLimit = 50*p;
    int iterationsWithoutImprovement = 0;
    int bestPenalization = evaluationFunction(initialRunway, p);

    while (iterationsWithoutImprovement < noImprovementLimit) { // Mientras no se cumpla el limite de iteraciones sin mejora, seguir buscando
        tabu.search(runway, p, p/10);
        int currentPenalization = evaluationFunction(runway, p);
        if (currentPenalization < bestPenalization) { // Resetear el valor de busquedas sin mejora
            bestPenalization = currentPenalization;
            iterationsWithoutImprovement = 0;
        } else {
            iterationsWithoutImprovement++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = end - start;

    int totalPenalization = evaluationFunction(runway, p);

    // Print Greedy Solution
    std::cout << std::endl << "Greedy Solution:" << std::endl;
    initialRunway.printRunwaySchedule();
    std::cout << std::endl;
    std::cout << "Costo Total: " << evaluationFunction(initialRunway, p) << std::endl << std::endl << std::endl;

    // Print new solution
    std::cout << std::endl << "Tabu-Search Solution:" << std::endl;
    runway.printRunwaySchedule();
    std::cout << std::endl;
    std::cout << "Costo Total: " << totalPenalization << std::endl;
    std::cout << "Tiempo de Computo: " << elapsedTime.count() << " [seg]" << std::endl;

    return 0;
}
