#include <iostream>

#include "bilinear_transform/bilinear_transform.h"

int main()
{
    const auto integrator_laplace = 1/s;

    constexpr auto bilinear_transform_approximation = 
        (2 * (Z - 1)) / 
        (T * (Z + 1));
    
    const auto first_order_approx_z_transform = 
        substitute(integrator_laplace, s, bilinear_transform_approximation);

    const auto z_transform = 
        extract_rational_fraction(first_order_approx_z_transform, Z); 

    std::cout << "transf(" << integrator_laplace << ") :\n" <<
                 "substitute : " << first_order_approx_z_transform << 
                 "\nextract frac" << z_transform << std::endl;

    return 0;
}

