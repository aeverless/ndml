export module ndml.vec:operations;

import :types;
import std;

export namespace ndml
{
/**
 * @brief Checks if the vector is reciprocatable.
 *
 * @tparam N     dimension
 * @tparam T     element type
 *
 * @param v vector to be checked
 *
 * @return @c true if @p v contains no zero elements, @c false otherwise
 */
template <std::size_t N, typename T>
constexpr auto is_reciprocatable(vec<N, T> const& v) noexcept -> bool
{
	return std::ranges::all_of(
		v,
		[](T component)
		{
			return component != T{};
		}
	);
}

/**
 * @brief Checks if the vector is normal.
 *
 * @tparam N     dimension
 * @tparam T     element type
 *
 * @param v vector to be checked
 *
 * @return @c true if @c norm_squared(v) is 1, @c false otherwise
 */
template <std::size_t N, typename T>
constexpr auto is_normal(
	vec<N, T> const& v,
	typename vec<N, T>::value_type const& tolerance = std::numeric_limits<T>::epsilon() * static_cast<T>(4 * N)
) noexcept -> bool
{
	if constexpr (std::floating_point<T>)
	{
		return std::abs(norm_squared(v) - T{1}) <= tolerance;
	}
	else
	{
		return norm_squared(v) == T{1};
	}
}

/**
 * @brief Checks if the vector is zero.
 *
 * @tparam N     dimension
 * @tparam T     element type
 *
 * @param v vector to be checked
 *
 * @return @c true if @c norm_squared(v) is 0, @c false otherwise
 */
template <std::size_t N, typename T>
constexpr auto is_zero(vec<N, T> const& v, typename vec<N, T>::value_type const& tolerance = std::numeric_limits<T>::epsilon() * static_cast<T>(4 * N)) noexcept -> bool
{
	if constexpr (std::floating_point<T>)
	{
		return norm_squared(v) <= tolerance;
	}
	else
	{
		return std::ranges::all_of(
			v,
			[](T component) static noexcept
			{
				return component == T{};
			}
		);
	}
}

/**
 * @brief Calls the function for every component, potentially modifying the vector.
 *
 * This calls @p f with each component of @p v.
 *
 * @tparam N     dimension
 * @tparam T     element type
 * @tparam Unary type of functor
 *
 * @param lhs vector to be potentially modified
 * @param f   functor
 *
 * @return reference to @p lhs
 */
template <std::size_t N, typename T, typename UnaryFn>
constexpr auto for_each(vec<N, T>& v, UnaryFn const& f) noexcept(noexcept(std::ranges::for_each(v, f))) -> vec<N, T>&
{
	std::ranges::for_each(v, f);
	return v;
}

/**
 * @brief Calls the function for pairs of components, potentially modifying @p lhs.
 *
 * This calls @p f with each component of @p lhs and @p rhs.
 *
 * @tparam N        dimension
 * @tparam T        element type
 * @tparam BinaryFn type of functor
 *
 * @param lhs vector to be potentially modified
 * @param rhs vector to call @p f with
 * @param f   functor
 *
 * @return reference to @p lhs
 */
template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_for_each(vec<N, T>& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>&
{
	for (auto i = 0uz; i < N; ++i)
	{
		f(lhs[i], rhs[i]);
	}

	return lhs;
}

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
 * @return transformed copy of @p v
 */
template <std::size_t N, typename T, typename UnaryFn>
constexpr auto transform(vec<N, T> const& v, UnaryFn const& f) noexcept(noexcept(f(v[0]))) -> vec<N, T>
{
	vec<N, T> result;

	for (auto i = 0uz; i < N; ++i)
	{
		result[i] = f(v[i]);
	}

	return result;
}

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
 * @return transformed copy of @p v
 */
template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_transform(vec<N, T> const& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>
{
	vec<N, T> result;

	for (auto i = 0uz; i < N; ++i)
	{
		result[i] = f(lhs[i], rhs[i]);
	}

	return result;
}

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
constexpr auto dot(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>::value_type
{
	T s{};

	for (auto i = 0uz; i < N; ++i)
	{
		s += lhs[i] * rhs[i];
	}

	return s;
}

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
constexpr auto cross(vec<3, T> const& lhs, vec<3, T> const& rhs) noexcept -> vec<3, T>
{
	return {
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x,
	};
}

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
constexpr auto norm_squared(vec<N, T> const& v) noexcept -> vec<N, T>::value_type
{
	return dot(v, v);
}

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
constexpr auto norm(vec<N, T> const& v) noexcept -> std::common_type_t<T, double>
{
	auto norm_squared = std::common_type_t<T, double>{0.0};

	for (auto i = 0uz; i < N; ++i)
	{
		norm_squared += v[i] * static_cast<std::common_type_t<T, double>>(v[i]);
	}

	return std::sqrt(norm_squared);
}

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
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto normal(vec<N, T> const& v) noexcept -> vec<N, T> pre(!is_zero(v)) post(r : is_normal(r))
{
	return v / norm(v);
}

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
 * @pre @p axis is normalized
 *
 * @return @p v projected onto @p axis
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto projection(vec<N, T> const& v, vec<N, T> const& axis) noexcept -> vec<N, T> pre(is_normal(axis))
{
	return axis * dot(v, axis) / dot(axis, axis);
}

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
 * @pre @p v is reciprocatable
 *
 * @return the reciprocal of @p v
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto reciprocal(vec<N, T> const& v) noexcept -> vec<N, T> pre(is_reciprocatable(v))
{
	return transform(
		v,
		[](typename vec<N, T>::value_type component) static noexcept pre(component != T{})
		{
			return static_cast<T>(1) / component;
		}
	);
}

/**
 * @brief Vector addition assignment operator.
 *
 * Adds components of @p rhs to respective components of @p lhs.
 */
template <std::size_t N, typename T>
constexpr auto operator+=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::addition_assignment<T>{});
}

/**
 * @brief Vector subtraction assignment operator.
 *
 * Subtracts components of @p rhs from respective components of @p lhs.
 */
template <std::size_t N, typename T>
constexpr auto operator-=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::subtraction_assignment<T>{});
}

/**
 * @brief Vector multiplication assignment operator.
 *
 * Multiplies components of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::multiplication_assignment<T>{});
}

/**
 * @brief Vector division assignment operator.
 *
 * Divides components of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>& pre(is_reciprocatable(rhs))
{
	return zip_for_each(lhs, rhs, meta::division_assignment<T>{});
}

/**
 * @brief Vector-scalar multiplication assignment operator.
 *
 * Multiplies components of @p v by @p scale.
 */
template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&
{
	return for_each(
		v,
		[&scale](auto& component)
		{
			component *= scale;
		}
	);
}

/**
 * @brief Vector-scalar division assignment operator.
 *
 * Divides components of @p v by @p scale.
 */
template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>& pre(scale != T{})
{
	return for_each(
		v,
		[&scale](auto& component)
		{
			component /= scale;
		}
	);
}

/**
 * @brief Vector equality comparison operator.
 *
 * Compares components of @p lhs with components of @p rhs.
 *
 * @return @c true if two vectors are equal, @c false otherwise
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator==(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool
{
	return std::ranges::equal(lhs, rhs);
}

/**
 * @brief Vector inequality comparison operator.
 *
 * Compares components of @p lhs with components of @p rhs.
 *
 * @return @c false if two vectors are equal, @c true otherwise
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator!=(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool
{
	return !(lhs == rhs);
}

/**
 * @brief Vector promotion operator.
 *
 * Returns a copy of @p v with unary promotion applied to each of its components.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator+(vec<N, T> const& v) noexcept -> vec<N, T>
{
	return transform(
		v,
		[](T component)
		{
			return component = +component;
		}
	);
}

/**
 * @brief Vector negation operator.
 *
 * Returns a copy of @p v with unary negation applied to each of its components.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator-(vec<N, T> const& v) noexcept -> vec<N, T>
{
	return transform(
		v,
		[](T component)
		{
			return component = -component;
		}
	);
}

/**
 * @brief Vector addition operator.
 *
 * Adds components of @p rhs to respective components of a copy of @p lhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator+(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::plus<T>{});
}

/**
 * @brief Vector subtraction operator.
 *
 * Subtracts components of @p rhs from respective components of a copy of @p lhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator-(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::minus<T>{});
}

/**
 * @brief Vector multiplication operator.
 *
 * Multiplies components of a copy of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::multiplies<T>{});
}

/**
 * @brief Vector division operator.
 *
 * Divides components of a copy of @p lhs by respective components of @p rhs.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator/(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T> pre(is_reciprocatable(rhs))
{
	return zip_transform(lhs, rhs, std::divides<T>{});
}

/**
 * @brief Vector-scalar multiplication operator.
 *
 * Multiplies components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>
{
	return transform(
		v,
		[scale](typename vec<N, T>::value_type component)
		{
			return component * scale;
		}
	);
}

/**
 * @brief Scalar-vector multiplication operator.
 *
 * Multiplies components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator*(typename vec<N, T>::value_type const& scale, vec<N, T> const& v) noexcept -> vec<N, T>
{
	return v * scale;
}

/**
 * @brief Vector-scalar division operator.
 *
 * Divides components of a copy of @p v by @p scale.
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto operator/(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T> pre(scale != T{})
{
	return transform(
		v,
		[scale](typename vec<N, T>::value_type component)
		{
			return component / scale;
		}
	);
}
}
