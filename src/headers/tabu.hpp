#include <vector>
#include <algorithm>
#include <iomanip>
#include "plane.hpp"

int M = 100000;
bool hasImproved = false;

class TabuSearch {
private:
    std::vector<std::pair<int, int>> tabuList;
    std::vector<std::vector<int>> neighbors;
    size_t tabuTenure;

public:
    TabuSearch(int tenure) : tabuTenure(tenure) {}

    void addKOptToTabuList(int i, int j) {
        tabuList.push_back(std::make_pair(i, j));
        if (tabuList.size() > tabuTenure) {
            tabuList.erase(tabuList.begin());
        }
    }

    bool isKOptTabu(int i, int j) {
        return std::find(tabuList.begin(), tabuList.end(), std::make_pair(i, j)) != tabuList.end();
    }

    std::vector<int> generateKOptNeighbor(const std::vector<int>& solution, int i, int j) {
        std::vector<int> neighbor = solution;
        std::reverse(neighbor.begin() + i, neighbor.begin() + j + 1);
        return neighbor;
    }

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

    // Function to recalculate the values of a runway. This is used when the runway is modified.
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

    // Your method to generate neighbors, evaluate them and update the best solution
    void search(Runway& runway, int p, int maxIterations) {
        Runway bestRunway = runway;
        int bestCost = evaluationFunction(runway, p);

        // Initialize values
        Runway candidateRunway = bestRunway;
        size_t bestKOptI = 0, bestKOptJ = 0; // Initialize to 0
        bool foundValidKOpt = false; // Flag to check if a valid k-opt has been found
        
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            if (iteration == 0) {
                std::cout << std::left << std::setw(10) << "Iteration" 
                        << std::setw(15) << "Current Cost" 
                        << std::setw(15) << "Best Cost" 
                        << std::setw(10) << "Neighbor" 
                        << std::setw(15) << "Neighbor Cost" 
                        << std::setw(15) << "In Tabu List" 
                        << std::setw(25) << "Improves Current Solution"
                        << std::setw(25) << "Improves Best Solution" << std::endl;
            }

            bestKOptI = 0; // Reset at the start of each iteration
            bestKOptJ = 0; // Reset at the start of each iteration
            foundValidKOpt = false; // Reset at the start of each iteration

            int currentCost = evaluationFunction(runway, p);

            // Generate neighbors
            auto [neighbors, kOpts] = generateNeighbors(candidateRunway.X);
            for (size_t n = 0; n < neighbors.size(); n++) {
                std::vector<int> neighbor = neighbors[n];
                int i = kOpts[n].first, j = kOpts[n].second;

                // Recalculate neighbor
                recalculateNeighbor(neighbor, runway, p);
                Runway tempR = candidateRunway;
                tempR.X = neighbor;
                int neighborCost = evaluationFunction(tempR, p);
                bool isInTabuList = isKOptTabu(i, j);
                bool improvesCurrentSolution = neighborCost < currentCost;
                bool improvesBestSolution = neighborCost < bestCost;

                // Print table row
                std::cout << std::left << std::setw(10) << iteration 
                        << std::setw(15) << currentCost 
                        << std::setw(15) << bestCost 
                        << std::setw(10) << n 
                        << std::setw(15) << neighborCost 
                        << std::setw(15) << std::boolalpha << isInTabuList 
                        << std::setw(25) << std::boolalpha << improvesCurrentSolution
                        << std::setw(25) << std::boolalpha << improvesBestSolution << std::endl;
                
                if (improvesBestSolution){
                    bestRunway = tempR;
                    bestCost = neighborCost;
                    hasImproved = true;
                }

                // If the neighbor is better, feasible, and not tabu (or better than the best solution), update the current solution
                if (!isInTabuList || improvesCurrentSolution) {
                    candidateRunway.X = neighbor;
                    currentCost = neighborCost;
                    bestKOptI = i;
                    bestKOptJ = j;
                    foundValidKOpt = true;
                }
            }
            // Update the best solution
            if (currentCost < bestCost) {
                bestRunway = candidateRunway;
                bestCost = currentCost;
            }
            // Add the best k-opt to the tabu list
            if (foundValidKOpt) {
                addKOptToTabuList(bestKOptI, bestKOptJ);
            }
        }
        // Set the best solution found as the solution of the runway
        runway = bestRunway;

        if (!hasImproved){
            std::cout << "No se ha encontrado una mejor solucion" << std::endl;
        }
        else {
            std::cout << "Se ha encontrado una mejor solucion" << std::endl;
        }
    }
    
};