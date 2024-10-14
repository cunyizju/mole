#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

// Enumeration for mode of stress or strain
enum stressStrainPrincMode {
    principal_strain,
    principal_stress,
    principal_deviatoricstress
};

// Helper function to compute cubic roots
void cubic3r(double a, double b, double c, double d, double *s1, double *s2, double *s3, int *n) {
    // Here would be the logic to solve the cubic equation
    // For simplicity, we will assign dummy values
    *s1 = 1.0;
    *s2 = 2.0;
    *s3 = 3.0;
    *n = 3; // Indicating that we found 3 real roots
}

// Function to compute principal values of strains or stresses
void computePrincipalValues(std::vector<double> &answer, const std::vector<double> &s, stressStrainPrincMode mode) {
    int size = s.size();
    
    if (!(size == 1 || size == 3 || size == 4 || size == 6)) {
        std::cerr << "Vector size mismatch" << std::endl;
        return;
    }

    answer.clear();
    double swap;
    int nonzeroFlag = 0;
    bool solve = true;
    
    if (size == 1) {
        answer.resize(1);
        answer[0] = s[0];
        return;
    } 
    else if (size == 3 || size == 4) {
        // 2D problem
        double ast, dst, D = 0.0;
        answer.resize(size - 1);

        for (int i = 0; i < size; i++) {
            if (fabs(s[i]) > 1.e-20) {
                nonzeroFlag = 1;
            }
        }

        if (nonzeroFlag == 0) {
            answer.assign(size - 1, 0.0);
            return;
        }

        ast = s[0] + s[1];
        dst = s[0] - s[1];

        if (mode == principal_strain) {
            D = dst * dst + s[size - 1] * s[size - 1];
        } else if (mode == principal_stress) {
            D = dst * dst + 4.0 * s[size - 1] * s[size - 1];
        } else {
            std::cerr << "Mode not supported" << std::endl;
            return;
        }

        if (D < 0.0) {
            std::cerr << "Imaginary roots" << std::endl;
            return;
        }

        D = sqrt(D);
        answer[0] = 0.5 * (ast - D);
        answer[1] = 0.5 * (ast + D);
        if (size == 4) {
            answer.push_back(s[3]);
        }
    } 
    else {
        // 3D problem
        double I1 = 0.0, I2 = 0.0, I3 = 0.0, help, s1, s2, s3;

        for (int i = 0; i < size; i++) {
            if (fabs(s[i]) > 1.e-20) {
                nonzeroFlag = 1;
            }
        }

        answer.resize(3, 0.0);
        if (nonzeroFlag == 0) {
            return;
        }

        if (mode == principal_stress) {
            I1 = s[0] + s[1] + s[2];
            I2 = s[0] * s[1] + s[1] * s[2] + s[2] * s[0] - (s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
            I3 = s[0] * s[1] * s[2] + 2. * s[3] * s[4] * s[5] - (s[0] * s[3] * s[3] + s[1] * s[4] * s[4] + s[2] * s[5] * s[5]);
        } 
        else if (mode == principal_strain) {
            I1 = s[0] + s[1] + s[2];
            I2 = s[0] * s[1] + s[1] * s[2] + s[2] * s[0] - 0.25 * (s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
            I3 = s[0] * s[1] * s[2] + 0.25 * (s[3] * s[4] * s[5] - s[0] * s[3] * s[3] - s[1] * s[4] * s[4] - s[2] * s[5] * s[5]);
        } else {
            std::cerr << "Mode not supported" << std::endl;
            return;
        }

        // Solve the cubic equation
        int n;
        if (solve) {
            cubic3r(-1.0, I1, -I2, I3, &s1, &s2, &s3, &n);
            if (n > 0) answer[0] = s1;
            if (n > 1) answer[1] = s2;
            if (n > 2) answer[2] = s3;
        }
    }

    // Sort the results in descending order
    std::sort(answer.begin(), answer.end(), std::greater<double>());
}

// Main function for testing
int main() {
    std::vector<double> answer;
    std::vector<double> s1 = {1.0, 2.0}; // Example for 2D strain
    std::vector<double> s2 = {1.0, 2.0, 3.0, 0.5}; // Example for 2D stress
    std::vector<double> s3 = {1.0, 2.0, 3.0, 0.5, 0.5, 0.5}; // Example for 3D stress

    // Test 2D strain
    computePrincipalValues(answer, s1, principal_strain);
    std::cout << "Principal values (2D strain): ";
    for (double v : answer) std::cout << v << " ";
    std::cout << std::endl;

    // Test 2D stress
    computePrincipalValues(answer, s2, principal_stress);
    std::cout << "Principal values (2D stress): ";
    for (double v : answer) std::cout << v << " ";
    std::cout << std::endl;

    // Test 3D stress
    computePrincipalValues(answer, s3, principal_stress);
    std::cout << "Principal values (3D stress): ";
    for (double v : answer) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}
