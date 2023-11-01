#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>

const int VeryBigNumber = 1000;  // A very large number to use in separation constraints

// Function to calculate the penalization for a given plane's landing time
int calculatePenalization(int landingTime, int Ti, int gi, int hi) {
    if (landingTime < Ti) {
        return gi * (Ti - landingTime);
    }
    else {
        return hi * (landingTime - Ti);
    }
}

// Greedy algorithm to generate the initial solution
void greedyAlgorithm(std::vector<int>& solution, const std::vector<int>& E, const std::vector<int>& T, const std::vector<int>& L, const std::vector<int>& g, const std::vector<int>& h, const std::vector<std::vector<int>>& S) {
    for (size_t i = 0; i < solution.size(); i++) {
        int minPenalization = std::numeric_limits<int>::max();
        int bestLandingTime = -1;

        for (int landingTime = E[i]; landingTime <= L[i]; landingTime++) {
            // Calculate penalization for this landing time
            int penalization = calculatePenalization(landingTime, T[i], g[i], h[i]);

            // Check separation constraints
            bool validLanding = true;
            for (size_t j = 0; j < solution.size(); j++) {
                if (i != j) {
                    if (landingTime < solution[j] + S[i][j] || landingTime > solution[j] + S[j][i]) {
                        validLanding = false;
                        break;
                    }
                }
            }

            // Check if this landing time minimizes the penalization and satisfies separation constraints
            if (penalization < minPenalization && validLanding) {
                minPenalization = penalization;
                bestLandingTime = landingTime;
            }
        }

        // Assign the best landing time for plane i
        solution[i] = bestLandingTime;
    }
}