#ifndef NDML_MAT_OPERATION_HPP
#define NDML_MAT_OPERATION_HPP

#include "mat.hpp"

namespace ndml
{
/**
 * @brief The transpose of a matrix.
 *
 * This calculates the transpose of @p m, i.e. @p m with rows and columns swapped in order.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the transpose of @p m
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto transpose(mat<R, C, T> const& m) noexcept -> mat<C, R, T>;

/**
 * @brief The row echelon form of a matrix.
 *
 * This calculates the row echelon form of @p m, i.e. @p m with first non-zero entries
 * of all rows forming a staircase pattern.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the row echelon form of @p m
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto row_echelon_form(mat<N, N, T> const& m) noexcept -> mat<N, N, T>;

/**
 * @brief The determinant of a matrix.
 *
 * This calculates the determinant of a matrix.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the determinant of @p m
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto determinant(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type;

/**
 * @brief The inverse of a matrix.
 *
 * This calculates the inverse of a matrix via a method called Gauss-Jordan elimination.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the inverse of @p m
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto inverse(mat<N, N, T> const& m) noexcept -> mat<N, N, T>;

/**
 * @brief The trace of a matrix.
 *
 * This calculates the trace of a matrix by summing up its diagonal elements.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the trace of @p m
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto trace(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type;

/**
 * @brief Matrix addition assignment operator.
 *
 * Adds columns of @p rhs to respective columns of @p lhs.
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto operator+=(mat<R, C, T>& lhs, mat<R, C, T> const& rhs) noexcept -> mat<R, C, T>&;

/**
 * @brief Matrix subtraction assignment operator.
 *
 * Subtracts columns of @p rhs from respective columns of @p lhs.
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto operator-=(mat<R, C, T>& lhs, mat<R, C, T> const& rhs) noexcept -> mat<R, C, T>&;

/**
 * @brief Matrix multiplication assignment operator.
 *
 * Performs matrix multiplication for @p lhs and @p rhs and assigns the result to @p lhs.
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto operator*=(mat<R, C, T>& lhs, mat<R, C, T> const& rhs) noexcept -> mat<R, C, T>&;

/**
 * @brief Matrix-scalar multiplication assignment operator.
 *
 * Multiplies columns of @p lhs by @p scale.
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto operator*=(mat<R, C, T>& m, typename mat<R, C, T>::value_type const& scale) noexcept -> mat<R, C, T>&;

/**
 * @brief Matrix-scalar division assignment operator.
 *
 * Divides columns of @p lhs by @p scale.
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto operator/=(mat<R, C, T>& m, typename mat<R, C, T>::value_type const& scale) noexcept -> mat<R, C, T>&;

/**
 * @brief Matrix equality comparison operator.
 *
 * Compares columns of @p lhs with columns of @p rhs.
 *
 * @return @c true if two columns are equal, @c false otherwise
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator==(mat<R, C, T> const& lhs, mat<R, C, T> const& rhs) noexcept -> bool;

/**
 * @brief Matrix inequality comparison operator.
 *
 * Compares columns of @p lhs with columns of @p rhs.
 *
 * @return @c false if two columns are equal, @c true otherwise
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator!=(mat<R, C, T> const& lhs, mat<R, C, T> const& rhs) noexcept -> bool;

/**
 * @brief Matrix promotion operator.
 *
 * Returns a copy of @p m with unary promotion applied to each of its columns.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator+(mat<R, C, T> const& m) noexcept -> mat<R, C, T>;

/**
 * @brief Matrix promotion operator.
 *
 * Returns a copy of @p m with unary negation applied to each of its columns.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator-(mat<R, C, T> const& m) noexcept -> mat<R, C, T>;

/**
 * @brief Matrix addition operator.
 *
 * Adds columns of @p rhs to respective columns of a copy of @p lhs and returns the result.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator+(mat<R, C, T> const& lhs, mat<R, C, T> const& rhs) noexcept -> mat<R, C, T>;

/**
 * @brief Matrix subtraction operator.
 *
 * Subtracts columns of @p rhs from respective columns of a copy of @p lhs and returns the result.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator-(mat<R, C, T> const& lhs, mat<R, C, T> const& rhs) noexcept -> mat<R, C, T>;

/**
 * @brief Matrix multiplication operator.
 *
 * Performs matrix multiplication for @p lhs and @p rhs and returns the result.
 */
template <std::size_t N, std::size_t M, std::size_t K, typename T>
[[nodiscard]]
constexpr auto operator*(mat<N, M, T> const& lhs, mat<M, K, T> const& rhs) noexcept -> mat<N, K, T>;

/**
 * @brief Matrix-vector multiplication operator.
 *
 * Multiplies @p m by @p v and returns the result.
 */
template <std::size_t N, std::size_t M, typename T>
[[nodiscard]]
constexpr auto operator*(mat<N, M, T> const& m, vec<M, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Matrix-scalar multiplication operator.
 *
 * Multiplies @p m by @p scale and returns the result.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator*(mat<R, C, T> const& m, typename mat<R, C, T>::value_type const& scale) noexcept -> mat<R, C, T>;

/**
 * @brief Scalar-matrix multiplication operator.
 *
 * Multiplies @p m by @p scale and returns the result.
 */
template <std::size_t R, std::size_t C, typename T>
[[nodiscard]]
constexpr auto operator*(typename mat<R, C, T>::value_type const& scale, mat<R, C, T> const& m) noexcept -> mat<R, C, T>;
}

#include "operation.inl"

#endif
