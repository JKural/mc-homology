#include "../include/core/cubical_complex_3d.h"

#include "algebra/integer.h"
#include "algebra/z2_field.h"
#include "complexes/cubical_complex.h"

namespace core {

void CubicalComplex3D::add_cube(int x, int y, int z) {
    m_inner.add_recursive(product(
        product(
            complexes::CubicalSimplex::interval(x),
            complexes::CubicalSimplex::interval(y)
        ),
        complexes::CubicalSimplex::interval(z)
    ));
}

std::unique_ptr<Homology> CubicalComplex3D::z2_homology() const {
    return homology<algebra::Z2>();
}

std::unique_ptr<Homology> CubicalComplex3D::z3_homology() const {
    return homology<algebra::ZModP<3>>();
}

std::unique_ptr<Homology> CubicalComplex3D::z_homology() const {
    return homology<algebra::Integer>();
}

void CubicalComplex3D::reduce() {
    throw std::logic_error("Not implemented yet");
}

} // namespace core
