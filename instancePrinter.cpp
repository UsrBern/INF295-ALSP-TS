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
#include "src/headers/plane.hpp"

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

    // Print runway parameters
    std::cout << "Runway parameters:" << std::endl;
    Runway runway(planes);
    runway.print();   
}