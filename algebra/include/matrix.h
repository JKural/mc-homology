#pragma once

#include <concepts>
#include <format>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

#include "algebraic_concepts.h"

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

    /// \brief Return an identity matrix
    constexpr static Matrix id(size_type n)
        requires AbelianRing<T>
    {
        auto identity = zero(n);
        for (size_type i = 0; i < n; ++i) {
            identity[i, i] = value_type::one();
        }
        return identity;
    }

private:
    constexpr size_type
    to_underlying_index(size_type row, size_type col) const noexcept {
        return row * m_ncols + col;
    }

    storage_type m_data;
    size_type m_nrows;
    size_type m_ncols;
};

/// \brief Deduce matrix value type from the submitted range
template<std::ranges::sized_range R>
Matrix(R&&, std::size_t, std::size_t) -> Matrix<std::ranges::range_value_t<R>>;

template<class T>
std::ostream& operator<<(std::ostream& output, Matrix<T> const& matrix) {
    return output << std::format("{}", matrix);
}

} // namespace algebra

template<class T>
    requires std::formattable<T, char>
struct std::formatter<algebra::Matrix<T>> {
    bool one_line = true;
    std::formatter<T> coefficient_formatter;

    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
        auto it = ctx.begin();
        if (it == ctx.end() || *it == '}') {
            return it;
        } else if (*it == '-') {
            one_line = true;
            ++it;
        } else if (*it == '#') {
            one_line = false;
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

    template<class FmtContext>
    FmtContext::iterator
    format(algebra::Matrix<T> const& matrix, FmtContext& ctx) const {
        auto const spacer = one_line ? ' ' : '\n';
        auto const indent = one_line ? "" : " ";
        if (matrix.empty()) {
            std::format_to(ctx.out(), "[]");
        } else {
            std::format_to(ctx.out(), "{}[", spacer);
            for (std::size_t i = 0; i < matrix.nrows(); ++i) {
                if (i > 0) {
                    std::format_to(ctx.out(), "{}", indent);
                }
                std::format_to(ctx.out(), "[");
                for (std::size_t j = 0; j < matrix.ncols(); ++j) {
                    coefficient_formatter.format(matrix[i, j], ctx);
                    if (j + 1 < matrix.ncols()) {
                        std::format_to(ctx.out(), ", ");
                    }
                }
                std::format_to(ctx.out(), "]");
                if (i + 1 < matrix.nrows()) {
                    std::format_to(ctx.out(), ",{}", spacer);
                }
            }
            std::format_to(ctx.out(), "]{}", spacer);
        }
        if (!one_line) {
            std::format_to(
                ctx.out(),
                "Matrix {} x {}\n",
                matrix.nrows(),
                matrix.ncols()
            );
        }
        return ctx.out();
    }
};
