export module ndml.quat:types;

import ndml.vec;
import std;

export namespace ndml
{
/**
 * @brief Quaternion.
 *
 * A hypercomplex number consisting of four components.
 *
 * It is represented as @f$ x \mathbf i + y \mathbf j + z \mathbf k + w @f$, where @f$ x, y, z, w @f$ are values of type @p T,
 * and @f$ \mathbf i, \mathbf j, \mathbf k @f$ are the vectors of an orthonormal basis.
 *
 * @tparam T element type
 */
template <typename T>
struct quat : vec<4, T>
{
	using value_type = T;

	/**
	 * @brief Default constructor.
	 *
	 * Components are value-initialized.
	 */
	constexpr quat() noexcept = default;

	/**
	 * @brief Constructor from components.
	 *
	 * X, Y, Z, and W components are initialized to @p x, @p y, @p z, and @p w, respectively.
	 */
	constexpr quat(value_type x, value_type y, value_type z, value_type w) noexcept
		: vec<4, T>{std::move(x), std::move(y), std::move(z), std::move(w)}
	{}

	/**
	 * @brief Constructor from a three-dimensional vector (imaginary part) and a scalar (real part).
	 *
	 * X, Y, and Z components are initialized to the respective components of @p v,
	 * whilst the W component is initialized to @p w.
	 */
	constexpr quat(vec<3, value_type> v, value_type w) noexcept
		: quat{std::move(v.x), std::move(v.y), std::move(v.z), std::move(w)}
	{}

	/**
	 * @brief Converting constructor from a four-dimensional vector.
	 *
	 * Components are initialized to the respective components of @p v.
	 */
	constexpr quat(vec<4, value_type> v) noexcept
		: vec<4, value_type>{v}
	{}
};
}
