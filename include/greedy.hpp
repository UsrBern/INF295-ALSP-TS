#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include "../include/plane.hpp"

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
int greedyAlgorithm(std::vector<Plane>& planes) {
    int totalPenalization = 0;

    for (size_t i = 0; i < planes.size(); i++) {
        int bestLandingTime = planes[i].T;
        int minPenalization = calculatePenalization(planes[i].T, planes[i].T, planes[i].g, planes[i].h);

        for (int landingTime = planes[i].E; landingTime <= planes[i].L; landingTime++) {
            // Skip the ideal landing time (already initialized)
            if (landingTime == planes[i].T) {
                continue;
            }

            // Calculate penalization for this landing time
            int penalization = calculatePenalization(landingTime, planes[i].T, planes[i].g, planes[i].h);

            // Check separation constraints
            bool validLanding = true;
            for (size_t j = 0; j < planes.size(); j++) {
                if (i != j) {
                    if (landingTime < planes[j].assignedLandingTime + planes[i].S[j] || landingTime > planes[j].assignedLandingTime + planes[j].S[i]) {
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
        planes[i].assignedLandingTime = bestLandingTime;

        // Add the penalization to the total penalization
        totalPenalization += minPenalization;
    }

    // Return the total penalization
    return totalPenalization;
}

