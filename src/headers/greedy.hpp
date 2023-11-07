#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include "plane.hpp"

bool isSeparationRespected(Runway& runway, int i, int j, int newTime) {
    int Xi = runway.X[i];
    int Sij = runway.S[i][j];
    if (newTime < Xi + Sij) {
        return false;
    }
    return true;
}

// Greedy algorithm to generate the initial solution
void greedyAlgorithm(Runway& runway, int numPlanes) {
    // Extract T vector from runway object
    std::vector<int> T = runway.T;

    // Sort the planes vector by T
    // Sort the landing times vector (X) by T
    std::vector<int> sortedIndices(numPlanes);
    for (int i = 0; i < numPlanes; i++) {
        sortedIndices[i] = i;
    }
    std::sort(sortedIndices.begin(), sortedIndices.end(), [&T](int a, int b) {
        return T[a] < T[b];
    });

    // Initialize landing times vector with the runway's T values
    runway.X.resize(T.size());
    for (int i = 0; i < numPlanes; i++) {
        int index = sortedIndices[i];
        runway.X[index] = T[index];
    }

    // Separation of plane i and j is plane[i].S[j]
    // Iterate over all pairs of planes to ensure separation is respected
    bool separationViolated = true;
    while (separationViolated) {
        for (int p = 0; p < numPlanes; p++){
            separationViolated = false;
            int i = sortedIndices[p];
            int j = sortedIndices[p + 1];
            if (p + 1 == numPlanes) { // if we have reached the last plane in the sequence
                break;
            }
            
            int Sij = runway.S[i][j];
            int Xi = runway.X[i];
            int Xj = runway.X[j];

            // If the separation is not respected, alter landing times to respect it
            if (Xj < Xi + Sij) {
                separationViolated = true;
                if (runway.h[j] < runway.g[i]) { // if penalization for delaying j is smaller than penalization for advancing i
                    if (Xj + Sij <= runway.L[j] && isSeparationRespected(runway, i, j, Xj + Sij)) { // if delaying j does not violate its latest time window limit (L) and separation constraint with other planes, delay j
                        runway.X[j] = Xj + Sij;
                    }
                    else if (Xi - Sij >= runway.E[i] && isSeparationRespected(runway, i, j, Xi - Sij)) { // if advancing i does not violate its earliest time window limit (E) and separation constraint with other planes, advance i
                        runway.X[i] = Xi - Sij;
                    }
                    else { // else this means that the separation constraint cannot be respected
                        runway.X[j] = runway.L[j];
                    }
                }
                else { // else penalization for advancing i is smaller than penalization for delaying j
                    if (Xi + Sij <= runway.E[i] && isSeparationRespected(runway, i, j, Xi + Sij)) { // if advancing i does not violate its earliest time window limit (E) and separation constraint with other planes, advance i
                        runway.X[i] = Xi + Sij;
                    }
                    else if (Xj - Sij >= runway.L[j] && isSeparationRespected(runway, i, j, Xj - Sij)) { // if delaying j does not violate its latest time window limit (L) and separation constraint with other planes, delay j
                        runway.X[j] = Xj - Sij;
                    }
                    else { // else this means that the separation constraint cannot be respected
                        runway.X[i] = runway.E[i];
                    }
                }
            }
        }
    }
}


