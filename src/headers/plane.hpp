#ifndef PLANE_HPP
#define PLANE_HPP

#include <iostream>
#include <vector>
#include <iomanip>

// Clase: Plane
// Contiene la informacion de un solo avion
class Plane {
public:
    int E; // E: Tiempo mas temprano de aterrizaje del avion
    int T; // T: Tiempo ideal de aterrizaje del avion
    int L; // L: Tiempo mas tardio de aterrizaje del avion
    float g; // g: Penalizacion al avion por adelantarse a T
    float h; // h: Penalizacion al avion por atrasarse segun T
    std::vector<int> S; // S: Vector conteniendo la separacion minima requerida entre el avion actual y todos los otros
                        // Si el avion actual es el avion i, S[j-1] es la separacion minima requerida entre el avion i y el avion j

    // Constructor to initialize a Plane
    Plane(int e, int t, int l, int penaltyG, int penaltyH, const std::vector<int>& separations) 
        : E(e), T(t), L(l), g(penaltyG), h(penaltyH), S(separations) {
    }

    // Print para el avion (usado para debugging)
    void print() const {
        std::cout << "E: " << E << " T: " << T << " L: " << L << " g: " << g << " h: " << h << std::endl;
        for (size_t i = 0; i < S.size(); i++) {
            std::cout << "S" << i + 1 << ": " << S[i] << " ";
        }
        std::cout << std::endl;
    }
};

// Clase: Runway
// Contiene la informacion de todos los aviones a procesar
class Runway {
public:
    // Vectores de arametros E, T, L, g, h, S que significan lo mismo que en la clase Plane
    // La informacion contenida en el indice i-1 de cada vector corresponde a la informacion del avion i
    std::vector<int> E;
    std::vector<int> T;
    std::vector<int> L;
    std::vector<float> g;
    std::vector<float> h;
    std::vector<std::vector<int>> S;

    // Representacion:
    // Vector de tiempos de aterrizaje, X[i-1] corresponde al tiempo de aterrizaje asignado al avion i
    std::vector<int> X;

    // Constructor
    Runway(const std::vector<Plane>& planes) {
        for (const Plane& plane : planes) {
            E.push_back(plane.E);
            T.push_back(plane.T);
            L.push_back(plane.L);
            g.push_back(plane.g);
            h.push_back(plane.h);
            S.push_back(plane.S);
        }
    }

    // Copy constructor: Permite el uso de operaciones tipo `Runway runway2 = runway1`
    // Retorna la copia de la runway que se esta copiando
    Runway(const Runway& other) 
    : E(other.E), T(other.T), L(other.L), g(other.g), h(other.h), S(other.S), X(other.X) {}
    Runway& operator=(const Runway& other) {
    if (this != &other) {
        E = other.E;
        T = other.T;
        L = other.L;
        g = other.g;
        h = other.h;
        S = other.S;
        X = other.X;
    }
    return *this;
}
    
    // Funcion print de debugging
    void print() const {
        std::cout << "Plane parameters:" << std::endl;
        for (size_t i = 0; i < E.size(); i++) {
            std::cout << "E" << i + 1 << ": " << E[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < T.size(); i++) {
            std::cout << "T" << i + 1 << ": " << T[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < L.size(); i++) {
            std::cout << "L" << i + 1 << ": " << L[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < g.size(); i++) {
            std::cout << "g" << i + 1 << ": " << g[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < h.size(); i++) {
            std::cout << "h" << i + 1 << ": " << h[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < S.size(); i++) {
            std::cout << "S" << i + 1 << "j: ";
            for (size_t j = 0; j < S[i].size(); j++) {
                std::cout << S[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Funcion print que presenta la secuencia de aterrizaje en tiempo cronologico en forma de tabla vertical
    // (no se uso tabla horizontal porque resulto ser demasiado larga para ser legible en terminal en las instancias mas grandes)
    void printRunwaySchedule() const {
        // Extract landing times vector (X) from runway object
        std::vector<int> X = this->X;

        // Sort landing times vector (X), preserve their indices
        std::vector<int> sortedIndices(X.size());
        for (size_t i = 0; i < X.size(); i++) {
            sortedIndices[i] = i;
        }
        std::sort(sortedIndices.begin(), sortedIndices.end(), [&X](int i1, int i2) {
            return X[i1] < X[i2];
        });

        // Print runway schedule
        std::cout << "| Avion | Instante |\n";
        for (size_t i = 0; i < X.size(); i++) {
            std::cout << "| " << std::setw(5) << sortedIndices[i] + 1 << " | " << std::setw(8) << X[sortedIndices[i]] << " |\n";
        }
        std::cout << std::endl;
    }
};

// Funcion que calcula y retorna la penalizacion del avion i luego de recibir su landingTime (valor de runway.X[i]), y sus parametros T, g, y h.
int calculatePenalization(int landingTime, int Ti, int gi, int hi) {
    if (landingTime < Ti) {
        return gi * (Ti - landingTime);
    }
    else {
        return hi * (landingTime - Ti);
    }
}

// Funcion de evaluacion
// Recibe una runway y la cantidad total de aviones
// Calcula la penalizacion total de la secuencia de aterrizaje y la retorna
int evaluationFunction(const Runway& runway, int p) {
    int totalPenalization = 0;
    for (int i = 0; i < p; i++) {
        int penalization = calculatePenalization(runway.X[i], runway.T[i], runway.g[i], runway.h[i]);
        totalPenalization += penalization;
    }
    return totalPenalization;
}

void enforceConstraints(Runway& runway) {
    int numPlanes = runway.X.size();

    // Create a placeholder vector with indices of planes
    std::vector<int> indices(numPlanes);
    for (int i = 0; i < numPlanes; i++) {
        indices[i] = i;
    }

    // Sort the indices vector by the earliest landing times
    std::sort(indices.begin(), indices.end(), [&runway](int a, int b) {
        return runway.E[a] < runway.E[b];
    });

    // Initialize the landing time of the first plane
    runway.X[indices[0]] = runway.E[indices[0]];

    // Adjust the landing times of the subsequent planes
    for (int p = 1; p < numPlanes; p++) {
        int i = indices[p - 1];
        int j = indices[p];
        int Sij = runway.S[i][j];
        runway.X[j] = std::max(runway.E[j], runway.X[i] + Sij);
        if (runway.X[j] > runway.L[j]) {
            // The constraints cannot be satisfied
            std::cout << "The constraints cannot be satisfied" << std::endl;
            return;
        }
    }
}

#endif
