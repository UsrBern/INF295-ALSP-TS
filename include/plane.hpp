#include <iostream>
#include <vector>

class Plane {
public:
    int E;
    int T;
    int L;
    float g;
    float h;
    std::vector<int> S;
    int assignedLandingTime;
    int planeID;

    // Constructor to initialize a Plane
    Plane(int e, int t, int l, int penaltyG, int penaltyH, const std::vector<int>& separations, int ID) 
        : E(e), T(t), L(l), g(penaltyG), h(penaltyH), S(separations), assignedLandingTime(-1), planeID(ID) {
    }

    void print() const {
        std::cout << "E: " << E << " T: " << T << " L: " << L << " g: " << g << " h: " << h << std::endl;
        for (size_t i = 0; i < S.size(); i++) {
            std::cout << "S" << i + 1 << ": " << S[i] << " ";
        }
        std::cout << std::endl;
    }
};