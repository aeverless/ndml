#ifndef NDML_QUAT_OPERATION_HPP
#define NDML_QUAT_OPERATION_HPP

#include "quat.hpp"

namespace ndml
{
/**
 * @brief Conjugate of a quaternion.
 *
 * This calculates a quaternion of form @f$ (-x, -y, -z, w) @f$.
 *
 * @return the conjugate of @p q
 */
template <typename T>
[[nodiscard]]
constexpr auto conjugate(quat<T> const& q) noexcept -> quat<T>;

/**
 * @brief Inverse of a quaternion.
 *
 * This calculates a quaternion such that multiplication of @p q by it
 * results in an identity quaternion.
 *
 * @return the conjugate of @p q divided by its squared norm
 */
template <typename T>
[[nodiscard]]
constexpr auto inverse(quat<T> const& q) noexcept -> quat<T>;

/**
 * @brief Axis and angle of a versor.
 *
 * This extracts the axis and angle out of a versor.
 * For quaternions with an imaginary part nearing zero, return value is value-initialized.
 *
 * @return the axis-angle pair of @p q
 */
template <typename T>
[[nodiscard]]
constexpr auto axis_angle(quat<T> const& q) noexcept -> std::pair<vec<3, T>, T>;

/**
 * @brief Quaternion Hamilton multiplication assignment operator.
 *
 * Calculates the Hamilton product of @p lhs and @p rhs and assigns the result to @p lhs.
 */
template <typename T>
constexpr auto operator*=(quat<T>& lhs, quat<T> const& rhs) noexcept -> quat<T>&;

/**
 * @brief Quaternion Hamilton multiplication operator.
 *
 * Calculates the Hamilton product of @p lhs and @p rhs.
 */
template <typename T>
[[nodiscard]]
constexpr auto operator*(quat<T> const& lhs, quat<T> const& rhs) noexcept -> quat<T>;

/**
 * @brief Conjugation of a vector by quaternion.
 *
 * Conjugates @p v by @p q, effectively rotating @p v if @p q is a versor.
 */
template <typename T>
[[nodiscard]]
constexpr auto operator*(quat<T> const& q, vec<3, T> const& v) noexcept -> vec<3, T>;
}

#include "operation.inl"

#endif
