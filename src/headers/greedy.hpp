#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include "plane.hpp"

// Greedy algorithm to generate the initial solution
void greedyAlgorithm(Runway& runway, int numPlanes) {
    std::cout << runway.S[10][2] << std::endl;
    // Extract T vector from runway object
    std::vector<int> T = runway.T;

    // Pair up the original indices with the T vector
    std::vector<std::pair<int, int>> pairs(T.size());
    for (size_t i = 0; i < T.size(); i++) {
        pairs[i] = std::make_pair(T[i], i + 1); // +1 if your indices are 1-based
    }

    // Sort the pairs vector
    std::sort(pairs.begin(), pairs.end());

    // Extract the sorted T and sortedIndices vectors
    std::vector<int> sortedT(T.size());
    std::vector<int> sortedIndices(T.size());
    for (size_t i = 0; i < pairs.size(); i++) {
        sortedT[i] = pairs[i].first;
        sortedIndices[i] = pairs[i].second;
    }
    // Initialize landing times vector with the runway's T values
    runway.X.resize(numPlanes);
    for (int i = 0; i < numPlanes; i++) {
        int index = sortedIndices[i];
        runway.X[index] = sortedT[i];
    }

    // Separation of plane i and j is plane[i].S[j]
    // Iterate over all pairs of planes to ensure separation is respected
    bool separationViolated = true;
    while (separationViolated) {
        std::cout << "Separation violated. Adjusting landing times..." << std::endl;
        separationViolated = false;
        for (int p = 0; p < numPlanes; p++){
            int i = sortedIndices[p];
            int j;
            if (p + 1 < numPlanes) {
                j = sortedIndices[p + 1];
            } else {
                break;
            }
            std::cout << "i: " << i << " j: " << j << std::endl;

            int Sij = runway.S[i][j];
            int Xi = runway.X[i];
            int Xj = runway.X[j];

            // If the separation is not respected, alter landing times to respect it
            if (Xj < Xi + Sij) {
                separationViolated = true;
                if (runway.h[j] < runway.g[i] && Xj + Sij <= runway.L[j]){ // if penalization for delaying j is smaller than penalization for advancing i
                    runway.X[j] = Xj + Sij; // delay j    
                }
                else if (Xi - Sij >= runway.E[i]) { // else penalization for advancing i is smaller than penalization for delaying j
                    runway.X[i] = Xi - Sij; // advance i
                }
            }
        }
    }
}


