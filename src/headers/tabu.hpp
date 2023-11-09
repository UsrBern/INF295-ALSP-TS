#include <vector>
#include <algorithm>
#include <iomanip>
#include "plane.hpp"

int M = 100000;


// Clase: TabuSearch
// Contiene la informacion y funciones requeridas para el algoritmo TabuSearch
class TabuSearch {
private:
    std::vector<std::pair<int, int>> tabuList; // Vector que actua como lista tabu de movimientos 2-opt, representados por los pares de indices se usaron para la operacion
    std::vector<std::vector<int>> neighbors; // Vector para contener a los vecinos durante la ejecucion de la busqueda
    size_t tabuTenure; // Largo de la lista tabu

public:
    // Constructor
    TabuSearch(int tenure) : tabuTenure(tenure) {}

    // Recibe los indices de un movimiento 2-opt y los añade a la lista tabu
    // Respeta la naturaleza FIFO de la lista
    void addKOptToTabuList(int i, int j) {
        tabuList.push_back(std::make_pair(i, j));
        if (tabuList.size() > tabuTenure) {
            tabuList.erase(tabuList.begin());
        }
    }

    // Recibe los indices de un movimiento 2-opt, retorna true si el movimiento es tabu, false si no
    bool isKOptTabu(int i, int j) {
        return std::find(tabuList.begin(), tabuList.end(), std::make_pair(i, j)) != tabuList.end();
    }

    // Recibe los indices sobre los cuales efectuar el movimiento 2-opt, retorna el resultado
    std::vector<int> generateKOptNeighbor(const std::vector<int>& solution, int i, int j) {
        std::vector<int> neighbor = solution;
        std::reverse(neighbor.begin() + i, neighbor.begin() + j + 1);
        return neighbor;
    }

    // Recibe un vector de integers que representa la solucion actual
    // crea su vecindario, y retorna un par conteniendo el vecindario (vector de integers)
    // y otro par con los indices i y j utilizados para crear cada vecino del vecindario (en el mismo orden que el vector del vecindario)
    std::pair<std::vector<std::vector<int>>, std::vector<std::pair<int, int>>> generateNeighbors(const std::vector<int>& solution) {
        std::vector<std::vector<int>> neighbors;
        std::vector<std::pair<int, int>> kOpts;
        for (size_t i = 0; i < solution.size(); i++) {
            for (size_t j = i + 1; j < solution.size(); j++) {
                std::vector<int> neighbor = generateKOptNeighbor(solution, i, j);
                neighbors.push_back(neighbor);
                kOpts.push_back(std::make_pair(i, j));
            }
        }
        return std::make_pair(neighbors, kOpts);
    }

    // Funcion que recibe un vector solucion (neighbor) ordenado cronologicamente, la runway y la cantidad total de aviones
    // Void: no retorna nada, pero altera el vector solucion obligando a los tiempos asignados a respetar
    // las restricciones de rango de tiempo de aterrizaje y separacion entre aviones
    void recalculateNeighbor(std::vector<int>& neighbor, Runway& runway, int p){
        // Check range constraints
        for (int i = 0; i < p; i++){
            if (neighbor[i] < runway.E[i]){
                neighbor[i] = runway.E[i];
            }
            else if (neighbor[i] > runway.L[i]){
                neighbor[i] = runway.L[i];
            }
        }

        // Check separation constraints
        std::vector<int> temp = neighbor;
        std::vector<int> sortedIndices(p);
        for (int k = 0; k < p; k++) {
            sortedIndices[k] = k;
        }
        std::sort(sortedIndices.begin(), sortedIndices.end(), [&temp](int i1, int i2) {
            return temp[i1] < temp[i2];
        });
        for (int k = 0; k < p; k++) {
            if (k+1 == p) {
                break;
            }
            size_t i = sortedIndices[k], j = sortedIndices[k+1];
            if (temp[i] < temp[j] + runway.S[i][j]) {
                if (runway.h[j] < runway.g[i]){
                    neighbor[j] = temp[i] + runway.S[i][j];
                }
                else {
                    neighbor[i] = temp[j] - runway.S[i][j];
                }
            }
        }

        // Check range constraints again
        for (int i = 0; i < p; i++){
            if (neighbor[i] < runway.E[i]){
                neighbor[i] = runway.E[i];
            }
            else if (neighbor[i] > runway.L[i]){
                neighbor[i] = runway.L[i];
            }
        }
    }

    // Funcion que efectua la Tabu-Search sobre la runway que recibe, sirviendose del int p conteniendo el total de aviones
    // y del int maxIterations que indica el numero maximo de iteraciones a generar en esta busqueda
    // Void: no retorna nada pero afecta el vector X de Runway si es que encuentra una mejor solucion que la que este vector contiene al ser llamada
    void search(Runway& runway, int p, int maxIterations) {
        bool hasImproved = false;
        bool hasInformed = false;

        Runway bestRunway = runway;
        int bestCost = evaluationFunction(runway, p);
        int initCost = bestCost;
        bool improvesBestSolution = false;

        // Initialize values
        Runway candidateRunway = bestRunway;
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            // Initialize the best non-tabu neighbor and its cost
            std::vector<int> bestNeighbor;
            int bestNeighborCost = std::numeric_limits<int>::max();
            int bestNeighborI = -1, bestNeighborJ = -1;

            // Generate neighbors
            auto [neighbors, kOpts] = generateNeighbors(candidateRunway.X);
            for (size_t n = 0; n < neighbors.size(); n++) {
                std::vector<int> neighbor = neighbors[n];
                int i = kOpts[n].first, j = kOpts[n].second;

                // Recalculate neighbor
                enforceConstraints(candidateRunway);
                Runway tempR = candidateRunway;
                tempR.X = neighbor;

                int neighborCost = evaluationFunction(tempR, p);
                bool isInTabuList = isKOptTabu(i, j);
                improvesBestSolution = neighborCost < bestCost;
                if (improvesBestSolution) {
                    hasImproved = true;
                }

                // Print the debugging information
                /*std::cout << std::setw(10) << iteration << " | " 
                        << std::setw(10) << bestCost << " | " 
                        << std::setw(14) << evaluationFunction(candidateRunway, p) << " | " 
                        << std::setw(14) << neighborCost << " | " 
                        << std::setw(5) << std::boolalpha << isInTabuList << " | " 
                        << std::setw(18) << (neighborCost < evaluationFunction(candidateRunway, p)) << " | " 
                        << std::setw(14) << improvesBestSolution << std::endl;
                */
                // If the neighbor is not tabu and better than the best non-tabu neighbor found so far, update the best non-tabu neighbor
                if (!isInTabuList && neighborCost < bestNeighborCost) {
                    bestNeighbor = neighbor;
                    bestNeighborCost = neighborCost;
                    bestNeighborI = i;
                    bestNeighborJ = j;
                }

                // Update the best solution
                if (improvesBestSolution) {
                    bestRunway = tempR;
                    bestCost = neighborCost;
                }
            }

            // Update the current solution to be the best non-tabu neighbor found
            if (!bestNeighbor.empty()) {
                candidateRunway.X = bestNeighbor;
                addKOptToTabuList(bestNeighborI, bestNeighborJ);
            }
        }
        // Set the best solution found as the solution of the runway
        runway = bestRunway;

        if (hasImproved && !hasInformed){
            hasInformed = true;
            std::cout << "Se ha encontrado una mejor solucion: " << initCost << " -> " << bestCost << std::endl;
            bestRunway.printRunwaySchedule();
        }
    }
    
};
