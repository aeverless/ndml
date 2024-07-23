#ifndef NDML_QUAT_TRANSFORM_HPP
#define NDML_QUAT_TRANSFORM_HPP

#include "quat.hpp"

namespace ndml
{
/**
 * @brief Versor from axis and angle.
 *
 * This initializes a quaternion equal to @f$ \cos \frac \theta 2 + u \sin \frac \theta 2 @f$,
 * where @f$ u @f$ is equal to @p axis and @f$ \theta @f$ is equal to @p angle.
 *
 * @param axis  versor axis
 * @param angle angle in radians
 */
template <typename T>
[[nodiscard]]
constexpr auto versor(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> quat<T>;

/**
 * @brief Quaternion to matrix conversion.
 *
 * This extracts axis and angle from @p q and creates a rotation matrix from them.
 */
template <typename T>
[[nodiscard]]
constexpr auto rotation(quat<T> const& q) noexcept -> mat<4, 4, T>;
}

#include "transform.inl"

#endif
