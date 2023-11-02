#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <regex>
#include "../include/greedy.hpp"

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
            inputFile.open("../instances/" + fileName);
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
    std::vector<int> solution(p, -1);
    auto start = std::chrono::high_resolution_clock::now();
    int totalPenalization = greedyAlgorithm(runway, p);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = end - start;

    // Print solution
    runway.printRunwaySchedule();
    std::cout << std::endl;

    std::cout << "Total Cost: " << totalPenalization << std::endl;
    std::cout << "Computation Time: " << elapsedTime.count() * 1000 << " ms" << std::endl;

    return 0;
}
