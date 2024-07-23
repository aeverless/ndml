#ifndef NDML_MAT_TRANSFORM_HPP
#define NDML_MAT_TRANSFORM_HPP

#include "mat.hpp"

namespace ndml
{
/**
 * @brief Vector cross product matrix.
 *
 * This calculates a matrix @c m such that multiplying it by a vector @c u
 * gives cross product of @p v and @c u.
 *
 * @tparam T element type
 *
 * @param v vector
 *
 * @return the cross matrix of @p v
 */
template <typename T>
[[nodiscard]]
constexpr auto cross_matrix(vec<3, T> const& v) noexcept -> mat<3, 3, T>;

/**
 * @brief Vector outer product matrix.
 *
 * This calculates the outer product matrix of @p lhs and @p rhs.
 *
 * @tparam T element type
 *
 * @param lhs left-side vector
 * @param rhs right-side vector
 *
 * @return the outer product of @p lhs and @p rhs
 */
template <std::size_t N, std::size_t M, typename T>
[[nodiscard]]
constexpr auto outer_product(vec<N, T> const& lhs, vec<M, T> const& rhs) noexcept -> mat<M, N, T>;

/**
 * @brief Scale matrix.
 *
 * This calculates the matrix such that multiplying it by a vector @c u
 * gives a vector @c u scaled component-wise by a vector @c v.
 *
 * @tparam T element type
 *
 * @param v scale vector
 *
 * @return the scale matrix of @p v
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto scale(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>;

/**
 * @brief Translation matrix.
 *
 * This calculates the matrix such that multiplying it by a vector @c u
 * gives a vector @f$ u + v @f$.
 *
 * @tparam T element type
 *
 * @param v translation vector
 *
 * @return the translation matrix of @p v
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto translation(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>;

/**
 * @brief Two-dimensional rotation matrix.
 *
 * This calculates the matrix such that multiplying it by a two-dimensional homogeneous vector @c u
 * gives a vector @c u rotated @c angle radians in the two-dimensional Cartesian plane.
 *
 * @tparam T element type
 *
 * @param angle angle in radians
 *
 * @return the two-dimensional rotation by angle @p angle matrix
 */
template <typename T>
[[nodiscard]]
constexpr auto rotation(T const& angle) noexcept -> mat<3, 3, T>;

/**
 * @brief Three-dimensional rotation matrix.
 *
 * This calculates the matrix such that multiplying it by a three-dimensional homogeneous vector @c u
 * gives a vector @c u rotated @c angle radians along the axis @p axis.
 *
 * @tparam T element type
 *
 * @param axis  rotation axis
 * @param angle angle in radians
 *
 * @return the three-dimensional rotation by angle @p angle along axis @p axis matrix
 */
template <typename T>
[[nodiscard]]
constexpr auto rotation(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> mat<4, 4, T>;

/**
 * @brief Look-at matrix.
 *
 * This calculates the change of basis matrix such that multiplying it by a vector @p u
 * gives the representation of @p u in the standard orthonormal basis.
 *
 * @tparam T element type
 *
 * @param eye    sight origin
 * @param target sight target
 * @param up     normal sight up direction
 *
 * @return the look-at matrix pointing at @p target from @p eye with its up direction equal to @p up
 */
template <typename T>
[[nodiscard]]
constexpr auto look_at(vec<3, T> const& eye, vec<3, T> const& target, vec<3, T> const& up) noexcept -> mat<4, 4, T>;

/**
 * @brief Orthogonal projection matrix.
 *
 * This calculates the orthogonal projection matrix delimited by near and far planes and rectangular bounds.
 *
 * @tparam T element type
 *
 * @param left   left bound
 * @param right  right bound
 * @param bottom bottom bound
 * @param up     up bound
 * @param near   near plane depth
 * @param far    far plane depth
 *
 * @return orthogonal projection matrix
 */
template <typename T>
[[nodiscard]]
constexpr auto ortho(T const& left, T const& right, T const& bottom, T const& up, T const& near, T const& far) noexcept -> mat<4, 4, T>;

/**
 * @brief Perspective projection matrix.
 *
 * This calculates the perspective projection matrix delimited by near and far planes
 * given a vertical FOV and aspect ratio.
 *
 * @tparam T element type
 *
 * @param vertical_fov vertical field of view in radians
 * @param aspect_ratio width to height ratio
 * @param near         near plane depth
 * @param far          far plane depth
 *
 * @return perspective projection matrix
 */
template <typename T>
[[nodiscard]]
constexpr auto perspective(T const& vertical_fov, T const& aspect_ratio, T const& near, T const& far) noexcept -> mat<4, 4, T>;
}

#include "transform.inl"

#endif
