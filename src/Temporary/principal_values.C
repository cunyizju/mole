#include <cmath>
#include <algorithm>
#include <iostream>
#include <array>

// Inline clamp function
inline double clamp(double a, double lower, double upper)
{
    return (a <= lower ? lower : (a >= upper ? upper : a));
}

// StructuralMaterial class definition
class StructuralMaterial
{
public:
    std::array<double, 3> computePrincipalValues(double I1, double I2, double I3)
    {
        double CUBIC_ZERO = 1e-100;
        double q = (I1 * I1 - 3.0 * I2) / 9.0;
        double r = (-2.0 * I1 * I1 * I1 + 9.0 * I1 * I2 - 27.0 * I3) / 54.0;
        double a3 = I1 / 3.0;

        // Hydrostatic case, in such a case q=r=0
        if ((fabs(q) < CUBIC_ZERO) && (fabs(r) < CUBIC_ZERO)) {
            return {a3, a3, a3};
        }

        // Three real roots (clamping to prevent rounding errors)
        double help = clamp(r / sqrt(q * q * q), -1., 1.);
        double phi = acos(help) / 3.0;
        double p = 2.0 * sqrt(q);

        std::array<double, 3> v = {
            a3 - p * cos(phi - 2. * M_PI / 3.),
            a3 - p * cos(phi),
            a3 - p * cos(phi + 2. * M_PI / 3.)
        };

        // Sort the values in ascending order
        if (v[0] > v[1]) std::swap(v[0], v[1]);
        if (v[1] > v[2]) std::swap(v[1], v[2]);
        if (v[0] > v[1]) std::swap(v[0], v[1]);

        return v;
    }
};

int main() 
{
    StructuralMaterial material;
    
    // Test case 1
    std::array<double, 3> result1 = material.computePrincipalValues(3.0, 2.0, 1.0);
    std::cout << "Principal values (test 1): " << result1[0] << ", " << result1[1] << ", " << result1[2] << std::endl;

    // Test case 2
    std::array<double, 3> result2 = material.computePrincipalValues(5.0, 3.0, 2.0);
    std::cout << "Principal values (test 2): " << result2[0] << ", " << result2[1] << ", " << result2[2] << std::endl;

    // Test case 3 (Hydrostatic case)
    std::array<double, 3> result3 = material.computePrincipalValues(1.0, 1.0 / 3.0, 1.0 / 27.0);
    std::cout << "Principal values (test 3): " << result3[0] << ", " << result3[1] << ", " << result3[2] << std::endl;

    return 0;
}
