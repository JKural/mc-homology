/// \file matrix.h
/// \brief A file containing matrix implementation

#pragma once

#include <algorithm>
#include <format>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

#include "algebra/algebraic_concepts.h"

namespace algebra {

/// \brief A Matrix class
///
/// A two-dimensional array representing a mathematical matrix.
template<class T>
class Matrix {
public:
    /// \brief Underlying storage type
    using storage_type = std::vector<T>;
    /// \brief Type of the stored values
    using value_type = std::vector<T>::value_type;
    /// \brief Type of the used allocator
    using allocator_type = std::vector<T>::allocator_type;
    /// \brief Size type used by the underlying storage
    using size_type = std::vector<T>::size_type;
    /// \brief Difference type used by the underlying storage
    using difference_type = std::vector<T>::difference_type;
    /// \brief Reference type to the stored values
    using reference = std::vector<T>::reference;
    /// \brief Const reference type to the stored values
    using const_reference = std::vector<T>::const_reference;
    /// \brief Pointer type to the stored values
    using pointer = std::vector<T>::pointer;
    /// \brief Const pointer type to the stored values
    using const_pointer = std::vector<T>::const_pointer;
    /// \brief Iterator over the underlying storage
    using iterator = std::vector<T>::iterator;
    /// \brief Const iterator over the underlying storage
    using const_iterator = std::vector<T>::const_iterator;
    /// \brief Reverse iterator over the underlying storage
    using reverse_iterator = std::vector<T>::reverse_iterator;
    /// \brief Const reverse iterator over the underlying storage
    using const_reverse_iterator = std::vector<T>::const_reverse_iterator;

    constexpr Matrix() = default;

    /// \brief Construct a matrix from a range
    ///
    /// Create a matrix with coefficients taken from the range and with
    /// the specified number of rows and columns. Size of the range has
    /// be equal to the product of nrows and ncols.
    ///
    /// \param data Range with the coefficients
    /// \param nrows Number of rows
    /// \param ncols Number of colums
    template<std::ranges::sized_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, T>
    constexpr explicit Matrix(R&& data, size_type nrows, size_type ncols) :
        m_data {},
        m_nrows {nrows},
        m_ncols {ncols} {
        if (std::ranges::size(data) != nrows * ncols) [[unlikely]] {
            throw std::domain_error(
                "Size of the array is not equal to the number"
                " of rows times the number of columns"
            );
        }
        m_data = std::ranges::to<std::vector<T>>(std::forward<R>(data));
    }

    /// \brief Iterator over the coefficients
    constexpr iterator begin() noexcept {
        return m_data.begin();
    }

    /// \brief Iterator over the coefficients
    constexpr const_iterator begin() const noexcept {
        return m_data.begin();
    }

    /// \brief Const iterator over the coefficients
    constexpr const_iterator cbegin() const noexcept {
        return m_data.cbegin();
    }

    /// \brief Sentinel for the coefficients iterator
    constexpr iterator end() noexcept {
        return m_data.end();
    }

    /// \brief Sentinel for the coefficients iterator
    constexpr const_iterator end() const noexcept {
        return m_data.end();
    }

    /// \brief Sentinel for the coefficients const iterator
    constexpr const_iterator cend() const noexcept {
        return m_data.cend();
    }

    /// \brief Reverse iterator over the coefficients
    constexpr reverse_iterator rbegin() noexcept {
        return m_data.rbegin();
    }

    /// \brief Reverse iterator over the coefficients
    constexpr const_reverse_iterator rbegin() const noexcept {
        return m_data.rbegin();
    }

    /// \brief Const reverse iterator over the coefficients
    constexpr const_reverse_iterator crbegin() const noexcept {
        return m_data.crbegin();
    }

    /// \brief Sentinel for the coefficients reverse iterator
    constexpr reverse_iterator rend() noexcept {
        return m_data.rend();
    }

    /// \brief Sentinel for the coefficients reverse iterator
    constexpr const_reverse_iterator rend() const noexcept {
        return m_data.rend();
    }

    /// \brief Sentinel for the coefficients const reverse iterator
    constexpr const_reverse_iterator crend() const noexcept {
        return m_data.crend();
    }

    /// \brief Test, if the matrix is empty
    constexpr bool empty() const noexcept {
        return m_data.empty();
    }

    /// \brief Number of elements in the matrix
    constexpr size_type size() const noexcept {
        return m_data.size();
    }

    /// \brief Specialized swap algorithm for the matrix
    constexpr void swap(Matrix& other) noexcept(
        m_data.swap(std::declval<std::vector<value_type>>())
    ) {
        namespace rs = std::ranges;
        m_data.swap(other.m_data);
        rs::swap(m_nrows, other.m_nrows);
        rs::swap(m_ncols, other.m_ncols);
    }

    /// \brief Number of rows
    constexpr size_type nrows() const noexcept {
        return m_nrows;
    }

    /// \brief Number of columns
    constexpr size_type ncols() const noexcept {
        return m_ncols;
    }

    /// \brief Equality comparison for matrices
    constexpr bool operator==(Matrix const&) const = default;

    /// \brief Access element at row `row` and columns `col`
    ///
    /// \param row Accessed row
    /// \param col Accessed column
    constexpr reference operator[](size_type row, size_type col) {
        if (row >= m_nrows || col >= m_ncols) [[unlikely]] {
            throw std::out_of_range("Indices out of matrix range");
        }
        return m_data[to_underlying_index(row, col)];
    }

    /// \brief Access element at row `row` and columns `col`
    ///
    /// \param row Accessed row
    /// \param col Accessed column
    constexpr const_reference operator[](size_type row, size_type col) const {
        if (row >= m_nrows || col >= m_ncols) [[unlikely]] {
            throw std::out_of_range("Indices out of matrix range");
        }
        return m_data[to_underlying_index(row, col)];
    }

    /// \brief Access element at row `row` and columns `col`
    ///
    /// \param row Accessed row
    /// \param col Accessed column
    constexpr reference at(size_type row, size_type col) {
        if (row >= m_nrows || col >= m_ncols) [[unlikely]] {
            throw std::out_of_range("Indices out of matrix range");
        }
        return m_data.at(to_underlying_index(row, col));
    }

    /// \brief Access element at row `row` and columns `col`
    ///
    /// \param row Accessed row
    /// \param col Accessed column
    constexpr const_reference at(size_type row, size_type col) const {
        if (row >= m_nrows || col >= m_ncols) [[unlikely]] {
            throw std::out_of_range("Indices out of matrix range");
        }
        return m_data.at(to_underlying_index(row, col));
    }

    /// \brief Direct to underlying storage
    constexpr storage_type const& data() const noexcept {
        return m_data;
    }

    /// \brief The transpose of the matrix
    constexpr Matrix transpose() const {
        auto transposed = zero(m_ncols, m_nrows);
        for (size_type i = 0; i < m_nrows; ++i) {
            for (size_type j = 0; j < m_ncols; ++j) {
                transposed[j, i] = at(i, j);
            }
        }
        return transposed;
    }

    /// \brief Adds rhs to itself
    constexpr Matrix& operator+=(Matrix const& rhs)
        requires AdditiveGroup<T>
    {
        if (m_nrows != rhs.m_nrows || m_ncols != rhs.m_ncols) {
            throw std::domain_error("Adding matrices of different dimensions");
        }
        namespace rs = std::ranges;
        rs::transform(m_data, rhs.m_data, rs::begin(m_data), std::plus {});
        return *this;
    }

    /// \brief Subtracts rhs from itself
    constexpr Matrix& operator-=(Matrix const& rhs)
        requires AdditiveGroup<T>
    {
        if (m_nrows != rhs.m_nrows || m_ncols != rhs.m_ncols) {
            throw std::domain_error(
                "Subtracting matrices of different dimensions"
            );
        }
        namespace rs = std::ranges;
        rs::transform(m_data, rhs.m_data, rs::begin(m_data), std::minus {});
    }

    /// \brief Returns a copy of itself
    constexpr Matrix operator+() const
        requires AdditiveGroup<T>
    {
        return *this;
    }

    /// \brief Returns a negation of itself
    constexpr Matrix operator-() const
        requires AdditiveGroup<T>
    {
        namespace rs = std::ranges;
        std::vector<T> negated;
        negated.reserve(m_data.capacity());
        rs::transform(m_data, rs::begin(negated), std::negate {});
        return Matrix(negated, m_nrows, m_ncols);
    }

    /// \brief Multiplies itself by rhs
    ///
    /// Matrix multiplies itself from the right-hand side by rhs.
    constexpr Matrix operator*=(Matrix const& rhs)
        requires Ring<T>
    {
        *this = *this * rhs;
    }

    /// \brief Return a square zero matrix
    constexpr static Matrix zero(size_type n)
        requires AdditiveGroup<T>
    {
        std::vector data(n * n, value_type::zero());
        return Matrix(std::move(data), n, n);
    }

    /// \brief Return a rectangle zero matrix
    constexpr static Matrix zero(size_type n, size_type m)
        requires AdditiveGroup<T>
    {
        std::vector data(n * m, value_type::zero());
        return Matrix(std::move(data), n, m);
    }

    /// \brief Returns true if matrix is zero, false otherwise
    constexpr bool is_zero() const noexcept {
        return std::ranges::all_of(m_data, [](T const& x) {
            return x == T::zero();
        });
    }

    /// \brief Return an identity matrix
    constexpr static Matrix id(size_type n)
        requires CommutativeRing<T>
    {
        auto identity = zero(n);
        for (size_type i = 0; i < n; ++i) {
            identity[i, i] = value_type::one();
        }
        return identity;
    }

private:
    /// \brief Returns the underlying index of an element at given
    ///        coordinates
    constexpr size_type
    to_underlying_index(size_type row, size_type col) const noexcept {
        return row * m_ncols + col;
    }

    /// \brief Stored coefficients
    storage_type m_data;
    /// \brief Number of rows
    size_type m_nrows;
    /// \brief Number of columns
    size_type m_ncols;
};

/// \brief Adds two matrices
template<AdditiveGroup T>
constexpr Matrix<T> operator+(Matrix<T> lhs, Matrix<T> const& rhs) {
    return lhs += rhs;
}

/// \brief Subtracts two matrices
template<AdditiveGroup T>
constexpr Matrix<T> operator-(Matrix<T> lhs, Matrix<T> const& rhs) {
    return lhs -= rhs;
}

/// \brief Multiplies two matrices
///
/// Multiplies two matrices using the usual matrix multiplication.
template<Ring T>
constexpr Matrix<T> operator*(Matrix<T> const& lhs, Matrix<T> const& rhs) {
    using Matrix = Matrix<T>;
    using size_type = Matrix::size_type;
    if (lhs.ncols() != rhs.nrows()) {
        throw std::domain_error(
            "The number of columns of lhs is different "
            "than the number of rows of rhs"
        );
    }
    auto const inner_dim = lhs.ncols();
    Matrix product = Matrix::zero(lhs.nrows(), rhs.ncols());
    for (size_type i = 0; i < product.nrows(); ++i) {
        for (size_type j = 0; j < product.ncols(); ++j) {
            for (size_type k = 0; k < inner_dim; ++k) {
                product[i, j] += lhs[i, k] * rhs[k, j];
            }
        }
    }
    return product;
}

/// \brief Deduce matrix value type from the submitted range
template<std::ranges::sized_range R>
Matrix(R&&, std::size_t, std::size_t) -> Matrix<std::ranges::range_value_t<R>>;

/// \brief Outputs a matrix to a stream
template<class T>
std::ostream& operator<<(std::ostream& output, Matrix<T> const& matrix) {
    return output << std::format("{}", matrix);
}

} // namespace algebra

/// \brief Formatter for the `Matrix` type
///
/// Impletementation of the formatter for the `Matrix` type.
///
/// # Format syntax
///
/// 1. :[-|#][&ltcofficient_format_string&gt]
///
/// where &ltcoefficient_format_string&gt is the format string for the
/// coefficient type. `-` or no specifier means that the matrix will
/// be printed in a single line, `#` will print the matrix in multiple
/// lines, making it easier to read.
///
/// # Example
///
/// For `Matrix<Integer> matrix`
/// 1. `std::format("{}", matrix)` outputs a single line matrix
/// 2. `std::format("{:-}", matrix)` also outputs a single line matrix
/// 3. `std::format("{:#}", matrix)` output a multi-line matrix
/// 4. `std::format("{::b}", matrix)` output a single line matrix of
///    integers in binary representation
/// 5. `std::format("{:#:x}", matrix)` output a multi-line matrix of
///    integers in hexadecimal representation
template<class T>
    requires std::formattable<T, char>
struct std::formatter<algebra::Matrix<T>> {
    /// \brief Parses a context string
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        auto it = ctx.begin();
        if (it == ctx.end() || *it == '}') {
            return it;
        } else if (*it == '-') {
            multi_line = false;
            ++it;
        } else if (*it == '#') {
            multi_line = true;
            ++it;
        }

        if (it == ctx.end() || *it == '}') {
            return it;
        } else if (*it == ':') {
            ctx.advance_to(++it);
            return coefficient_formatter.parse(ctx);
        } else {
            throw std::format_error("Invalid formatting option for Matrix");
        }
    }

    /// \brief Formats a matrix
    ///
    /// Formats a matrix. Rules for the format string are specified in
    /// the class documentation.
    template<class FmtContext>
    FmtContext::iterator
    format(algebra::Matrix<T> const& matrix, FmtContext& ctx) const {
        auto const maybe_new_line = multi_line ? "\n" : "";
        if (matrix.empty()) {
            std::format_to(ctx.out(), "[]");
        } else {
            std::format_to(ctx.out(), "[");
            for (std::size_t i = 0; i < matrix.nrows(); ++i) {
                std::format_to(ctx.out(), "[");
                for (std::size_t j = 0; j < matrix.ncols(); ++j) {
                    coefficient_formatter.format(matrix[i, j], ctx);
                    if (j + 1 < matrix.ncols()) {
                        std::format_to(ctx.out(), ", ");
                    }
                }
                std::format_to(ctx.out(), "]");
                if (i + 1 < matrix.nrows()) {
                    std::format_to(ctx.out(), ",{} ", maybe_new_line);
                }
            }
            std::format_to(ctx.out(), "]");
        }
        if (multi_line) {
            std::format_to(
                ctx.out(),
                "\nMatrix {} x {}\n",
                matrix.nrows(),
                matrix.ncols()
            );
        }
        return ctx.out();
    }

private:
    /// \brief Flag whether to print the matrix in multiple lines
    bool multi_line = false;
    /// \brief Formatter for the coefficients
    std::formatter<T> coefficient_formatter;
};
