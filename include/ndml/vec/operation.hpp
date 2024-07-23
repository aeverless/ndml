#ifndef NDML_VEC_OPERATION_HPP
#define NDML_VEC_OPERATION_HPP

#include "vec.hpp"

namespace ndml
{
/**
 * @brief Transforms the components of a vector.
 *
 * This applies @p f to each component of @p v.
 *
 * @tparam N       dimension
 * @tparam T       element type
 * @tparam UnaryFn type of transform functor
 *
 * @param v vector to be transformed
 * @param f transform functor
 *
 * @return reference to @p v
 */
template <std::size_t N, typename T, typename UnaryFn>
constexpr auto transform(vec<N, T>& v, UnaryFn const& f) noexcept(noexcept(std::ranges::for_each(v, f))) -> vec<N, T>&;

/**
 * @brief Zip-transforms the components of a vector with the components of another vector.
 *
 * This applies @p f to the respective pairs of components of @p lhs and @p rhs.
 *
 * @tparam N        dimension
 * @tparam T        element type
 * @tparam BinaryFn type of transform functor
 *
 * @param lhs vector to be transformed
 * @param rhs vector to transform @p lhs against
 * @param f   transform functor
 *
 * @return reference to @p v
 */
template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_transform(vec<N, T>& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>&;

/**
 * @brief Dot product of two vectors.
 *
 * This calculates the dot product of two vectors.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param lhs left-side vector
 * @param rhs right-side vector
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto dot(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>::value_type;

/**
 * @brief Cross product of two vectors.
 *
 * This calculates the cross product of two vectors.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param lhs left-side vector
 * @param rhs right-side vector
 */
template <typename T>
[[nodiscard]]
constexpr auto cross(vec<3, T> const& lhs, vec<3, T> const& rhs) noexcept -> vec<3, T>;

/**
 * @brief Squared norm of vector.
 *
 * This calculates the squared norm of @p v, which is equal to the dot product of @p v with itself.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param v vector
 *
 * @return the dot product of @p v with itself
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto norm_squared(vec<N, T> const& v) noexcept -> vec<N, T>::value_type;

/**
 * @brief Norm of vector.
 *
 * This calculates the norm of @p v, which is equal to the square root of the dot product of @p v with itself.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param v vector
 *
 * @return the square root of the dot product of @p v with itself.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto norm(vec<N, T> const& v) noexcept -> double;

/**
 * @brief Normalized vector.
 *
 * This calculates the unit vector facing in the same direction as @p v by dividing @v by its norm.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param v vector
 *
 * @return @p v divided by its norm
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto normal(vec<N, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Projection of a vector onto given axis.
 *
 * This calculates the projection of @p v onto @p axis.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param v    vector to be projected
 * @param axis projection axis
 *
 * @return @p v projected onto @p axis
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto projection(vec<N, T> const& v, vec<N, T> const& axis) noexcept -> vec<N, T>;

/**
 * @brief Reciprocal of a vector.
 *
 * This calculates a vector such that multiplication of @p v by it will result in a vector
 * consisting of multiplication identities of type @p T, e.g. a vector of 1's when @p T = @c float.
 *
 * @tparam N dimension
 * @tparam T element type
 *
 * @param v vector
 *
 * @return the reciprocal of @p v
 *
 * @warning Behavior is undefined if any of the components of @p v are zero.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto reciprocal(vec<N, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Vector addition assignment operator.
 *
 * Adds components of @p rhs to respective components of @p lhs.
 */
template <std::size_t N, typename T>
constexpr auto operator+=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&;

/**
 * @brief Vector subtraction assignment operator.
 *
 * Subtracts components of @p rhs from respective components of @p lhs.
 */
template <std::size_t N, typename T>
constexpr auto operator-=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&;

/**
 * @brief Vector multiplication assignment operator.
 *
 * Multiplies components of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&;

/**
 * @brief Vector division assignment operator.
 *
 * Divides components of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&;

/**
 * @brief Vector-scalar multiplication assignment operator.
 *
 * Multiplies components of @p v by @p scale.
 */
template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&;

/**
 * @brief Vector-scalar division assignment operator.
 *
 * Divides components of @p v by @p scale.
 */
template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&;

/**
 * @brief Vector equality comparison operator.
 *
 * Compares components of @p lhs with components of @p rhs.
 *
 * @return @c true if two vectors are equal, @c false otherwise
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator==(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool;

/**
 * @brief Vector inequality comparison operator.
 *
 * Compares components of @p lhs with components of @p rhs.
 *
 * @return @c false if two vectors are equal, @c true otherwise
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator!=(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool;

/**
 * @brief Vector promotion operator.
 *
 * Returns a copy of @p v with unary promotion applied to each of its components.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator+(vec<N, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Vector negation operator.
 *
 * Returns a copy of @p v with unary negation applied to each of its components.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator-(vec<N, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Vector addition operator.
 *
 * Adds components of @p rhs to respective components of a copy of @p lhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator+(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>;

/**
 * @brief Vector subtraction operator.
 *
 * Subtracts components of @p rhs from respective components of a copy of @p lhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator-(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>;

/**
 * @brief Vector multiplication operator.
 *
 * Multiplies components of a copy of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>;

/**
 * @brief Vector division operator.
 *
 * Divides components of a copy of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator/(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>;

/**
 * @brief Vector-scalar multiplication operator.
 *
 * Multiplies components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>;

/**
 * @brief Scalar-vector multiplication operator.
 *
 * Multiplies components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(typename vec<N, T>::value_type const& scale, vec<N, T> const& v) noexcept -> vec<N, T>;

/**
 * @brief Vector-scalar division operator.
 *
 * Divides components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator/(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>;
}

#include "operation.inl"

#endif
