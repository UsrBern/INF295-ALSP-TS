#include <vector>
#include <algorithm>
#include "plane.hpp"

int M = 100000;

class TabuSearch {
private:
    std::vector<std::pair<int, int>> tabuList;
    std::vector<std::vector<int>> neighbors;
    size_t tabuTenure;

public:
    TabuSearch(int tenure) : tabuTenure(tenure) {}

    void addSwapToTabuList(int i, int j) {
        if (tabuList.size() >= tabuTenure) {
            tabuList.erase(tabuList.begin());
        }
        tabuList.push_back(std::make_pair(i, j));
    }

    bool isSwapTabu(int i, int j) {
        return std::find(tabuList.begin(), tabuList.end(), std::make_pair(i, j)) != tabuList.end();
    }

    std::vector<int> swap(std::vector<int> solution, int i, int j) {
        std::vector<int> newSolution = solution;
        std::swap(newSolution[i], newSolution[j]);
        return newSolution;
    }

    std::pair<std::vector<std::vector<int>>, std::vector<std::pair<int, int>>> generateNeighbors(const std::vector<int>& solution) {
        std::vector<std::vector<int>> neighbors;
        std::vector<std::pair<int, int>> swaps;
        for (size_t i = 0; i < solution.size(); i++) {
            for (size_t j = i + 1; j < solution.size(); j++) {
                std::vector<int> neighbor = swap(solution, i, j);
                neighbors.push_back(neighbor);
                swaps.push_back(std::make_pair(i, j));
            }
        }
        return std::make_pair(neighbors, swaps);
    }

    // Your method to generate neighbors, evaluate them and update the best solution
    void search(Runway& runway, int p, int maxIterations) {
        Runway bestRunway = runway;
        int bestCost = evaluationFunction(runway, p);
        
        // Initialize candidate
        Runway candidateRunway = bestRunway;

        for (int iteration = 0; iteration < maxIterations; iteration++) {
            int currentCost = evaluationFunction(runway, p);
            size_t bestSwapI, bestSwapJ; // Declare variables to store the best swap

            // Generate neighbors
            auto [neighbors, swaps] = generateNeighbors(candidateRunway.X);

            for (int n = 0; n < p; n++) {
                std::vector<int> neighbor = neighbors[n];
                int i = swaps[n].first, j = swaps[n].second;

                // Check the constraints
                bool isFeasible = true;

                // Range Constraint: E[i] <= X[i] <= L[i]
                for (int k = 0; k < p; k++) {
                    if (neighbor[k] < runway.E[k] || neighbor[k] > runway.L[k]) {
                        isFeasible = false;
                        break;
                    }
                }

                // Separation Constraint: X[j] >= X[i] + S[i][j]
                // Sort neighbor into temp first
                // Temp is now sorted by runway sequence
                std::vector<int> temp = neighbor;
                std::vector<int> sortedIndices(p);
                for (int k = 0; k < p; k++) {
                    sortedIndices[k] = k;
                }
                std::sort(sortedIndices.begin(), sortedIndices.end(), [&temp](int i1, int i2) {
                    return temp[i1] < temp[i2];
                });
                for (int k = 0; k < p && isFeasible; k++) {
                    if (k+1 == p) {
                        break;
                    }
                    size_t m = sortedIndices[k], n = sortedIndices[k+1];
                    if (temp[k+1] < temp[k] + runway.S[m][n]) {
                        isFeasible = false;
                        break;
                    }
                }

                Runway tempR = candidateRunway;
                tempR.X = neighbor;
                int neighborCost = evaluationFunction(tempR, p);

                // If the neighbor is better, feasible, and not tabu (or better than the best solution), update the current solution
                if (isFeasible && neighborCost < currentCost && (!isSwapTabu(i, j))) {
                    candidateRunway.X = neighbor;
                    currentCost = evaluationFunction(candidateRunway, p);
                    bestSwapI = i;
                    bestSwapJ = j;
                }
            }

            // Update the best solution
            if (currentCost < bestCost) {
                bestRunway = candidateRunway;
                bestCost = currentCost;
            }

            // Add the best swap to the tabu list
            addSwapToTabuList(bestSwapI, bestSwapJ);
        }

        // Set the best solution found as the solution of the runway
        runway = bestRunway;
    }
    
};