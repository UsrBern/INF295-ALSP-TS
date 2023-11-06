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
        Runway bestRunway = runway; // Copy of the original runway
        int bestCost = evaluationFunction(runway, p);
        

        // Initialize s matrix
        std::vector<std::vector<int>> s(p, std::vector<int>(p, 0));

        for (int iteration = 0; iteration < maxIterations; iteration++) {
            Runway candidateRunway = bestRunway; // Copy of the best runway
            int currentCost = evaluationFunction(runway, p);
            size_t bestSwapI, bestSwapJ; // Declare variables to store the best swap

            // Generate neighbors by swapping each pair of times
            for (size_t i = 0; i < candidateRunway.X.size(); i++) {
                for (size_t j = i + 1; j < candidateRunway.X.size(); j++) {
                    Runway neighborRunway = candidateRunway; // Copy of the candidate runway
                    neighborRunway.X = swap(candidateRunway.X, i, j);

                    // Update s matrix
                    for (size_t k = 0; k < neighborRunway.X.size(); k++) {
                        for (size_t l = k + 1; l < neighborRunway.X.size(); l++) {
                            s[k][l] = (neighborRunway.X[k] < neighborRunway.X[l]) ? 1 : 0;
                        }
                    }

                    // Evaluate the neighbor
                    int neighborCost = evaluationFunction(neighborRunway, p);
                    std::vector<int> neighbor = neighborRunway.X;

                    // Check the constraints
                    bool isFeasible = true;

                    // Range Constraint: E[i] <= X[i] <= L[i]
                    for (size_t k = 0; k < neighbor.size(); k++) {
                        if (neighbor[k] < runway.E[k] || neighbor[k] > runway.L[k]) {
                            isFeasible = false;
                            break;
                        }
                    }

                    // Separation Constraint: X[j] >= X[i] + S[i][j] - M * s[i][j]
                    for (size_t k = 0; k < neighbor.size() && isFeasible; k++) {
                        for (size_t l = k + 1; l < neighbor.size(); l++) {
                            if (neighbor[l] < neighbor[k] + runway.S[k][l] - M * s[k][l]) {
                                isFeasible = false;
                                break;
                            }
                        }
                    }

                    // Succession Constraint: s[i][j] + s[i][j] <= 1
                    for (size_t k = 0; k < s.size() && isFeasible; k++) {
                        for (size_t l = 0; l < s[k].size(); l++) {
                            if (s[k][l] + s[l][k] > 1) {
                                isFeasible = false;
                                break;
                            }
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

            // Update candidateSolution for the next iteration
            candidateRunway = bestRunway;
        }

        // Set the best solution found as the solution of the runway
        runway = bestRunway;
    }
};