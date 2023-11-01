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
    int numPlanes = planes.size();

    // Sort the planes vector by T
    std::sort(planes.begin(), planes.end(), [](const Plane& a, const Plane& b) {
        return a.T < b.T;
    });

    // Create a vector to store the landing times
    // Representation: A vector storing the landing time for each plane, such that their index in the vector is their ID
    std::vector<int> landingTimes(numPlanes);

    // Initialize landing times vector with the planes' T values
    for (int i = 0; i < numPlanes; i++) {
        int id = planes[i].planeID;
        landingTimes[id] = planes[i].T;
        planes[i].assignedLandingTime = planes[i].T;
    }

    // Separation of plane i and j is plane[i].S[j]
    // Iterate over all pairs of planes to ensure separation is respected
    for (int i = 0; i < numPlanes; i++){
        int j = i + 1;
        if (j == numPlanes) { // if we have reached the last plane in the sequence
            break;
        }
        
        int Sij = planes[i].S[j];
        int Xi = landingTimes[planes[i].planeID];
        int Xj = landingTimes[planes[j].planeID];

        // If the separation is not respected, alter landing times to respect it
        if (!(Xj >= Xi + Sij)) {
            if (planes[j].h <= planes[i].g){ // if penalization for delaying j is smaller than penalization for advancing i
                if (Xj + Sij < planes[j].L) { // if delaying j does not violate its latest time window limit (L), delay j
                    landingTimes[planes[j].planeID] = Xj + Sij;
                    planes[j].assignedLandingTime = Xj + Sij;
                    totalPenalization += calculatePenalization(Xj + Sij, planes[j].T, planes[j].g, planes[j].h);
                }
                else if (Xi + Sij < planes[i].E) { // if advancing i does not violate its earliest time window limit (E), advance i
                    landingTimes[planes[i].planeID] = Xi + Sij;
                    planes[i].assignedLandingTime = Xi + Sij;
                    totalPenalization += calculatePenalization(Xi + Sij, planes[i].T, planes[i].g, planes[i].h);
                }
                else { // else this means that the separation constraint cannot be respected
                    std::cout << "Error: separation constraint cannot be respected for planes " << planes[i].planeID << " and " << planes[j].planeID << std::endl;
                    return -1;
                }
            }
            else { // else penalization for advancing i is smaller than penalization for delaying j
                if (Xi + Sij < planes[i].E) { // if advancing i does not violate its earliest time window limit (E), advance i
                    landingTimes[planes[i].planeID] = Xi + Sij;
                    planes[i].assignedLandingTime = Xi + Sij;
                    totalPenalization += calculatePenalization(Xi + Sij, planes[i].T, planes[i].g, planes[i].h);
                }
                else if (Xj + Sij < planes[j].L) { // if delaying j does not violate its latest time window limit (L), delay j
                    landingTimes[planes[j].planeID] = Xj + Sij;
                    planes[j].assignedLandingTime = Xj + Sij;
                    totalPenalization += calculatePenalization(Xj + Sij, planes[j].T, planes[j].g, planes[j].h);
                }
                else { // else this means that the separation constraint cannot be respected
                    std::cout << "Error: separation constraint cannot be respected for planes " << planes[i].planeID << " and " << planes[j].planeID << std::endl;
                    return -1;
                }
            }
        }


    }
    
}




