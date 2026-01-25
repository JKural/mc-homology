/// \file cubical_complex_3d.h
/// \brief A file containing a concrete class CubicalComplex3D
#pragma once

#include "complexes/compute_chain_complex.h"
#include "complexes/cubical_complex.h"
#include "core/algebra_homology.h"
#include "core/complex.h"

namespace core {

/// \brief A class representing a cubical complex in 3D space
class CubicalComplex3D: public Complex {
public:
    /// \brief Adds a cube to the complex
    void add_cube(int x, int y, int z);

    /// \brief Computes Z2 homology of the complex
    std::unique_ptr<Homology> z2_homology() const override;

    /// \brief Computes Z3 homology of the complex
    std::unique_ptr<Homology> z3_homology() const override;

    /// \brief Computes Z homology of the complex
    std::unique_ptr<Homology> z_homology() const override;

    /// \brief Computes homology of the complex for coefficients
    ///        of type T
    template<class T>
    std::unique_ptr<AlgebraHomology<T>> homology() const {
        return std::make_unique<AlgebraHomology<T>>(
            algebra::homology(complexes::compute_chain_complex<T>(m_inner))
        );
    }

    /// \brief Decreases the complex's size without changing its
    ///        homology
    void reduce() override;

private:
    /// \brief Inner representation of the cubical complex
    complexes::CubicalComplex m_inner;
};

} // namespace core
