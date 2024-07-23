#ifndef NDML_MAT_MAT_HPP
#define NDML_MAT_MAT_HPP

#include "ndml/vec.hpp"

#include <array>
#include <concepts>
#include <type_traits>

namespace ndml
{
/**
 * @brief Matrix.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @note Matrix is stored column-major, i.e. given matrix @c m, @c m[i] is the i-th column of @c m.
 */
template <std::size_t R, std::size_t C, typename T>
struct mat
{
	static_assert(R > 0 && C > 0);

	using vec_type    = vec<R, T>;
	using column_type = vec_type;
	using value_type  = column_type::value_type;

	/**
	 * @brief Number of rows.
	 *
	 * It is a positive integer.
	 */
	static constexpr auto row_count = R;

	/**
	 * @brief Number of columns.
	 *
	 * It is a positive integer.
	 */
	static constexpr auto column_count = C;

	/**
	 * @brief Size of matrix.
	 *
	 * This returns the number of columns as the matrix is stored column-major.
	 *
	 * @return the number of columns
	 */
	[[nodiscard]]
	static consteval auto size() noexcept -> std::size_t;

	/**
	 * @brief Default constructor.
	 *
	 * Columns are value-initialized, i.e. the matrix will be equal to the zero matrix.
	 */
	constexpr mat() noexcept = default;

	/**
	 * @brief Copy constructor.
	 *
	 * Columns are copied.
	 */
	constexpr mat(mat const& m) noexcept = default;

	/**
	 * @brief Move constructor.
	 *
	 * Columns are moved.
	 */
	constexpr mat(mat&& m) noexcept = default;

	/**
	 * @brief Constructor from columns.
	 *
	 * This will initialize columns to parameters with respect to their order.
	 *
	 * @tparam FromTs types of columns
	 */
	template <typename... FromTs>
	constexpr mat(FromTs... columns) noexcept
		requires (std::same_as<FromTs, column_type> && ...);

	/**
	 * @brief Constructor from scale.
	 *
	 * This will initialize all entries along the main diagonal to @p scale.
	 *
	 * @tparam FromTs types of columns
	 */
	template <typename FromT>
	constexpr explicit mat(FromT const& scale) noexcept
		requires std::convertible_to<FromT, value_type>;

	/**
	 * @brief Converting copy constructor from a matrix of another type.
	 *
	 * This will initialize common columns to columns of another matrix casted to @p column_type
	 * and value-initialize columns indices of which are greater than @p FromC.
	 *
	 * @tparam FromR number of rows in another matrix
	 * @tparam FromC number of columns in another matrix
	 * @tparam FromT element type of another matrix
	 *
	 * @param m matrix to be copied
	 */
	template <std::size_t FromR, std::size_t FromC, typename FromT>
	constexpr explicit mat(mat<FromR, FromC, FromT> const& m) noexcept
		requires (FromR <= R && FromC <= C && std::convertible_to<FromT, value_type>);

	/**
	 * @brief Converting move constructor from a matrix of another type.
	 *
	 * This will initialize common columns to rvalue references to columns of another matrix casted to @p column_type
	 * and value-initialize columns indices of which are greater than @p FromC.
	 *
	 * @tparam FromR number of rows in another matrix
	 * @tparam FromC number of columns in another matrix
	 * @tparam FromT element type of another matrix
	 *
	 * @param m matrix to be moved
	 */
	template <std::size_t FromR, std::size_t FromC, typename FromT>
	constexpr explicit mat(mat<FromR, FromC, FromT>&& m) noexcept
		requires (FromR <= R && FromC <= C && std::convertible_to<FromT, value_type>);

	/**
	 * @brief Copy-assignment operator.
	 *
	 * @param other matrix to be copied
	 */
	constexpr auto operator=(mat const& other) & noexcept -> mat& = default;

	/**
	 * @brief Move-assignment operator.
	 *
	 * @param other matrix to be moved
	 */
	constexpr auto operator=(mat&& other) & noexcept -> mat& = default;

	/**
	 * @brief Column subscript operator.
	 *
	 * This retrieves the column at index @p column.
	 *
	 * @param column column index
	 *
	 * @return column at index @p column
	 *
	 * @warning Behavior is undefined when @p column >= @p C
	 */
	[[nodiscard]]
	constexpr auto operator[](this auto&& self, std::size_t column) noexcept -> decltype(auto);

	/**
	 * @brief Element subscript operator.
	 *
	 * This retrieves the element at the intersection of column @p column and row @p row.
	 *
	 * @param column column index
	 * @param row    row index
	 *
	 * @return element at the intersection of column @p column and row @p row.
	 *
	 * @warning Behavior is undefined when @p column >= @p C or @p row >= @p R.
	 */
	[[nodiscard]]
	constexpr auto operator[](this auto&& self, std::size_t column, std::size_t row) noexcept -> decltype(auto);

	/**
	 * @brief Iterator to the first column.
	 *
	 * This creates a new iterator pointing to the first column of the matrix.
	 */
	[[nodiscard]]
	constexpr auto begin(this auto&& self) noexcept -> decltype(auto);

	/**
	 * @brief Past-the-end iterator.
	 *
	 * This creates a new iterator pointing one past the end of the matrix columns.
	 */
	[[nodiscard]]
	constexpr auto end(this auto&& self) noexcept -> decltype(auto);

	/**
	 * @brief Constant iterator to the first column.
	 *
	 * This creates a new constant iterator pointing to the first column of the matrix.
	 */
	[[nodiscard]]
	constexpr auto cbegin(this auto const& self) noexcept -> decltype(auto);

	/**
	 * @brief Constant past-the-end iterator.
	 *
	 * This creates a new constant iterator pointing one past the end of the matrix columns.
	 */
	[[nodiscard]]
	constexpr auto cend(this auto const& self) noexcept -> decltype(auto);

protected:
	/// Matrix columns.
	std::array<column_type, column_count> columns_{};
};

template <std::size_t R, std::size_t C, typename T>
mat(mat<R, C, T> const&) -> mat<R, C, T>;

template <std::size_t R, std::size_t C, typename T>
mat(mat<R, C, T>&&) -> mat<R, C, T>;

template <typename... FromTs>
mat(FromTs const&... columns) -> mat<std::common_type_t<FromTs...>::dimension, sizeof...(FromTs), typename std::common_type_t<FromTs...>::value_type>;

/**
 * @brief Swaps two matrices.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param lhs left-side matrix
 * @param rhs right-side matrix
 */
template <std::size_t R, std::size_t C, typename T>
constexpr auto swap(mat<R, C, T>& lhs, mat<R, C, T>& rhs) noexcept -> void;
}

#include "mat.inl"

#endif
