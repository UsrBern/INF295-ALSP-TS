#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include "../include/plane.hpp"

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
    int numPlanes = planes.size();

    // Sort the planes vector by T
    std::sort(planes.begin(), planes.end(), [](const Plane& a, const Plane& b) {
        return a.T < b.T;
    });

    // Initialize landing times vector with the planes' T values
    for (int i = 0; i < numPlanes; i++) {
        int id = planes[i].planeID;
        planes[i].assignedLandingTime = planes[i].T;
    }

    // Separation of plane i and j is plane[i].S[j]
    // Iterate over all pairs of planes to ensure separation is respected
    bool separationViolated = true;
    while (separationViolated) {
        separationViolated = false;
        for (int i = 0; i < numPlanes; i++){
            int j = i + 1;
            if (j == numPlanes) { // if we have reached the last plane in the sequence
                break;
            }
            
            int Sij = planes[i].S[j];
            int Xi = planes[i].assignedLandingTime;
            int Xj = planes[j].assignedLandingTime;

            // If the separation is not respected, alter landing times to respect it
            if (!(Xj >= Xi + Sij)) {
                separationViolated = true;
                if (planes[j].h <= planes[i].g){ // if penalization for delaying j is smaller than penalization for advancing i
                    if (Xj + Sij < planes[j].L) { // if delaying j does not violate its latest time window limit (L), delay j
                        planes[j].assignedLandingTime = Xj + Sij;
                    }
                    else if (Xi + Sij < planes[i].E) { // if advancing i does not violate its earliest time window limit (E), advance i
                        planes[i].assignedLandingTime = Xi + Sij;
                    }
                    else { // else this means that the separation constraint cannot be respected
                        std::cout << "Error: separation constraint cannot be respected for planes " << planes[i].planeID << " and " << planes[j].planeID << std::endl;
                        return -1;
                    }
                }
                else { // else penalization for advancing i is smaller than penalization for delaying j
                    if (Xi + Sij < planes[i].E) { // if advancing i does not violate its earliest time window limit (E), advance i
                        planes[i].assignedLandingTime = Xi + Sij;
                    }
                    else if (Xj + Sij < planes[j].L) { // if delaying j does not violate its latest time window limit (L), delay j
                        planes[j].assignedLandingTime = Xj + Sij;
                    }
                    else { // else this means that the separation constraint cannot be respected
                        std::cout << "Error: separation constraint cannot be respected for planes " << planes[i].planeID << " and " << planes[j].planeID << std::endl;
                        return -1;
                    }
                }
            }
        }
    }

    // Calculate the total penalization by iterating through the planes
    for (int i = 0; i < numPlanes; i++) {
        int Xi = planes[i].assignedLandingTime;
        int Ti = planes[i].T;
        int gi = planes[i].g;
        int hi = planes[i].h;
        
        // Calculate penalization for plane i and add it to the total penalization
        totalPenalization += calculatePenalization(Xi, Ti, gi, hi);
    }

    // Return the total penalization
    return totalPenalization;

    
}




