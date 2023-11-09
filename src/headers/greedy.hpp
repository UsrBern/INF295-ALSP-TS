#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include "plane.hpp"

// Algoritmo greedy para la solucion inicial
// Recibe objeto tipo Runway (contiene info de todos los aviones a aterrizar) y el numero de aviones
// Void: No retorna nada, altera el vector X de la runway directamente
void greedyAlgorithm(Runway& runway, int numPlanes) {
    // Extract T vector from runway object
    std::vector<int> T = runway.T;

    // Sort the planes vector by T
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

    // Enforce the constraints
    enforceConstraints(runway);
}