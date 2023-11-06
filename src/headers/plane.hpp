#include <iostream>
#include <vector>
#include <iomanip>

class Plane {
public:
    int E;
    int T;
    int L;
    float g;
    float h;
    std::vector<int> S;

    // Constructor to initialize a Plane
    Plane(int e, int t, int l, int penaltyG, int penaltyH, const std::vector<int>& separations) 
        : E(e), T(t), L(l), g(penaltyG), h(penaltyH), S(separations) {
    }

    void print() const {
        std::cout << "E: " << E << " T: " << T << " L: " << L << " g: " << g << " h: " << h << std::endl;
        for (size_t i = 0; i < S.size(); i++) {
            std::cout << "S" << i + 1 << ": " << S[i] << " ";
        }
        std::cout << std::endl;
    }
};

class Runway {
public:
    // Parameters: E, T, L, g, h, S
    // Index i of each vector contains plane i's information
    std::vector<int> E;
    std::vector<int> T;
    std::vector<int> L;
    std::vector<float> g;
    std::vector<float> h;
    std::vector<std::vector<int>> S;

    // Representation:
    // A vector of landing times, where X[i] contains the landing time of plane i
    // This is the actual runway landing sequence
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

    // Copy constructor
    Runway(const Runway& other)
        : X(other.X), T(other.T), g(other.g), h(other.h), E(other.E), L(other.L), S(other.S) {
        // Copy each member variable from the other Runway
    }

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

// Function to calculate the penalization for a given plane's landing time
int calculatePenalization(int landingTime, int Ti, int gi, int hi) {
    if (landingTime < Ti) {
        return gi * (Ti - landingTime);
    }
    else {
        return hi * (landingTime - Ti);
    }
}

// Evaluation Function
int evaluationFunction(const Runway& runway, int p) {
    int totalPenalization = 0;
    for (int i = 0; i < p; i++) {
        int penalization = calculatePenalization(runway.X[i], runway.T[i], runway.g[i], runway.h[i]);
        totalPenalization += penalization;
    }
    return totalPenalization;
}