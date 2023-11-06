#include <vector>
#include <algorithm>
#include "plane.hpp"

int M = 100000;

class TabuSearch {
private:
    std::vector<std::pair<int, int>> tabuList;
    std::vector<std::vector<int>> neighbors;
    int tabuTenure;

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

    std::vector<std::vector<int>> generateNeighbors(const std::vector<int>& solution) {
        std::vector<std::vector<int>> neighbors;
        for (size_t i = 0; i < solution.size(); i++) {
            for (size_t j = i + 1; j < solution.size(); j++) {
                std::vector<int> neighbor = swap(solution, i, j);
                neighbors.push_back(neighbor);
            }
        }
        return neighbors;
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

            // Generate neighbors by swapping each pair of times
            for (size_t i = 0; i < p; i++) {
                for (size_t j = i + 1; j < p; j++) {
                    Runway neighborRunway = candidateRunway; // Copy of the candidate runway
                    neighborRunway.X = swap(candidateRunway.X, i, j);

                    // Evaluate the neighbor
                    int neighborCost = evaluationFunction(neighborRunway, p);
                    std::vector<int> neighbor = neighborRunway.X;

                    // Sort neighbor by X[i]
                    // Neighbor is now sorted by runway sequence
                    std::vector<int> sortedIndices(p);
                    for (size_t k = 0; k < neighbor.size(); k++) {
                        sortedIndices[k] = k;
                    }
                    std::sort(sortedIndices.begin(), sortedIndices.end(), [&neighbor](int i1, int i2) {
                        return neighbor[i1] < neighbor[i2];
                    });

                    // Check the constraints
                    bool isFeasible = true;

                    // Range Constraint: E[i] <= X[i] <= L[i]
                    for (size_t k = 0; k < p; k++) {
                        size_t i = sortedIndices[k];
                        if (neighbor[k] < runway.E[i] || neighbor[k] > runway.L[i]) {
                            isFeasible = false;
                            break;
                        }
                    }

                    // Separation Constraint: X[j] >= X[i] + S[i][j]
                    for (size_t k = 0; k < p && isFeasible; k++) {
                        if (k+1 == p) {
                            break;
                        }
                        size_t i = sortedIndices[k], j = sortedIndices[k+1];
                        if (neighbor[k+1] < neighbor[k] + runway.S[i][j]) {
                            isFeasible = false;
                            break;
                        }
                    }

                    // If the neighbor is better, feasible, and not tabu (or better than the best solution), update the current solution
                    if (isFeasible && neighborCost < currentCost && (!isSwapTabu(i, j) || neighborCost < bestCost)) {
                        candidateRunway = neighborRunway;
                        currentCost = neighborCost;
                        bestSwapI = i;
                        bestSwapJ = j;
                    }
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