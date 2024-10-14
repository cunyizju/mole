#include <iostream>

/*
Ternary operator (?:): This is a shorthand for an if-else statement.
( a <= lower ? lower : ...): If a is less than or equal to lower, return lower.
( a >= upper ? upper : a ): If a is greater than or equal to upper, return upper. If neither condition is true, return a.
*/

// Inline function to clamp a value 'a' between 'lower' and 'upper'
inline double clamp(double a, double lower, double upper)
{
    return (a <= lower ? lower : (a >= upper ? upper : a));
}

int main() 
{
    // Test cases
    double value1 = 5.9, lower1 = 1.3, upper1 = 10.5;
    int value2 = 0, lower2 = 1, upper2 = 10;
    int value3 = 15, lower3 = 1, upper3 = 10;
    double value4 = 10.1, lower4 = 1, upper4 = 10.3;
    int value5 = -3, lower5 = -5, upper5 = 5;

    // Applying the clamp function and printing the results
    std::cout << "Clamp(" << value1 << ", " << lower1 << ", " << upper1 << ") = " 
              << clamp(value1, lower1, upper1) << std::endl;

    std::cout << "Clamp(" << value2 << ", " << lower2 << ", " << upper2 << ") = " 
              << clamp(value2, lower2, upper2) << std::endl;

    std::cout << "Clamp(" << value3 << ", " << lower3 << ", " << upper3 << ") = " 
              << clamp(value3, lower3, upper3) << std::endl;

    std::cout << "Clamp(" << value4 << ", " << lower4 << ", " << upper4 << ") = " 
              << clamp(value4, lower4, upper4) << std::endl;

    std::cout << "Clamp(" << value5 << ", " << lower5 << ", " << upper5 << ") = " 
              << clamp(value5, lower5, upper5) << std::endl;

    return 0;
}
