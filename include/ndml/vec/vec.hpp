#ifndef NDML_VEC_VEC_HPP
#define NDML_VEC_VEC_HPP

#include "ndml/meta/burn.hpp"

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace ndml
{
/**
 * @brief Algebraic vector.
 *
 * @tparam N size, a positive integer not greater than 4
 * @tparam T element type
 */
template <std::size_t N, typename T>
struct vec
{
	static_assert(0 < N && N <= 4);

	/**
	 * @brief Iterator.
	 *
	 * It is used for subscriptions and for iterating over a vector.
	 *
	 * @tparam V cv-qualified vector type
	 */
	template <typename V>
	struct iterator;

	using value_type      = T;
	using reference       = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_const_t<value_type>;

	/**
	 * @brief Type of a vector component at given index.
	 *
	 * This evaluates to @c value_type for @p I < @p N and to a burn type otherwise.
	 *
	 * @tparam I component index
	 *
	 * @note For @p I >= @p N, this evaluates to different empty burn types to motivate empty member optimisation.
	 *
	 * @sa ndml::meta::burn_t
	 */
	template <std::size_t I>
	using component_type = std::conditional_t<I < N, value_type, meta::burn_t<std::integral_constant<std::size_t, I>>>;

	/**
	 * @brief Type of a subscript operator evaluation result for a given cv-qualified vector type.
	 *
	 * @tparam V cv-qualified vector type
	 */
	template <typename V>
	using subscript_result = std::conditional_t<std::is_const_v<std::remove_reference_t<V>>, const_reference, reference>;

	/**
	 * @brief Number of components.
	 *
	 * It is a positive integer not greater than 4.
	 */
	static constexpr auto dimension = N;

	/**
	 * @brief Size of vector.
	 *
	 * @return the number of components
	 */
	[[nodiscard]]
	static consteval auto size() noexcept -> std::size_t;

	/**
	 * @brief The X component of the vector.
	 *
	 * It is usable for @p N > 0.
	 */
	[[no_unique_address]]
	component_type<0> x{};

	/**
	 * @brief The Y component of the vector.
	 *
	 * It is usable for @p N > 1.
	 */
	[[no_unique_address]]
	component_type<1> y{};

	/**
	 * @brief The Z component of the vector.
	 *
	 * It is usable for @p N > 2.
	 */
	[[no_unique_address]]
	component_type<2> z{};

	/**
	 * @brief The W component of the vector.
	 *
	 * It is usable for @p N > 3.
	 */
	[[no_unique_address]]
	component_type<3> w{};

	/**
	 * @brief Default constructor.
	 *
	 * Components are value-initialized.
	 */
	constexpr vec() noexcept = default;

	/**
	 * @brief Copy constructor.
	 *
	 * Components are copied.
	 */
	constexpr vec(vec const& v) noexcept = default;

	/**
	 * @brief Move constructor.
	 *
	 * Components are moved.
	 */
	constexpr vec(vec&& v) noexcept = default;

	/**
	 * @brief Constructor from components.
	 *
	 * This will initialize components to parameters with respect to their order.
	 *
	 * @tparam FromTs types of components
	 */
	template <typename... FromTs>
	constexpr vec(FromTs... components) noexcept
		requires (sizeof...(FromTs) <= N && (std::convertible_to<FromTs, value_type> && ...));

	/**
	 * @brief Constructor from scale.
	 *
	 * This will initialize components to the value of @p scale.
	 *
	 * @tparam FromT scale type
	 */
	template <typename FromT>
	constexpr explicit vec(FromT const& scale) noexcept
		requires std::convertible_to<FromT, value_type>;

	/**
	 * @brief Converting copy constructor from a vector of another type.
	 *
	 * This will initialize common components to components of another vector casted to @p value_type
	 * and value-initialize components indices of which are greater than @p FromN.
	 *
	 * @tparam FromN dimension of another vector
	 * @tparam FromT element type of another vector
	 *
	 * @param v vector be copied
	 */
	template <std::size_t FromN, typename FromT>
	constexpr explicit vec(vec<FromN, FromT> const& v) noexcept
		requires (FromN <= N && std::convertible_to<FromT, value_type>);

	/**
	 * @brief Converting move constructor from a vector of another type.
	 *
	 * This will initialize common components to rvalue references to components of another vector casted to @p value_type
	 * and value-initialize components indices of which are greater than @p FromN.
	 *
	 * @tparam FromN dimension of another vector
	 * @tparam FromT element type of another vector
	 *
	 * @param v vector be moved
	 */
	template <std::size_t FromN, typename FromT>
	constexpr explicit vec(vec<FromN, FromT>&& v) noexcept
		requires (FromN <= N && std::convertible_to<FromT, value_type>);

	/**
	 * @brief Copy-assignment operator.
	 *
	 * @param other vector to be copied
	 */
	constexpr auto operator=(vec const& other) & noexcept -> vec& = default;

	/**
	 * @brief Move-assignment operator.
	 *
	 * @param other vector to be moved
	 */
	constexpr auto operator=(vec&& other) & noexcept -> vec& = default;

	/**
	 * @brief Subscript operator.
	 *
	 * This retrieves the component, or a burn instance thereof, at index @p i.
	 *
	 * @tparam V cv-qualified vector type
	 *
	 * @param i component index
	 *
	 * @return component at index @p i, including a burn instance
	 *
	 * @throws @c std::out_of_range when @p i >= @p N
	 */
	template <typename V>
	[[nodiscard]]
	constexpr auto operator[](this V&& self, std::size_t i) -> subscript_result<V>;

	/**
	 * @brief Conversion operator to boolean.
	 *
	 * This returns @c true when any of the components is truthy and @c false otherwise.
	 */
	[[nodiscard]]
	explicit constexpr operator bool(this auto const& self) noexcept;

	/**
	 * @brief Iterator to the first component.
	 *
	 * This creates a new iterator pointing to the first component of the vector.
	 */
	[[nodiscard]]
	constexpr auto begin(this auto&& self) noexcept -> decltype(auto);

	/**
	 * @brief Past-the-end iterator.
	 *
	 * This creates a new iterator pointing one past the end of the vector components.
	 */
	[[nodiscard]]
	constexpr auto end(this auto&& self) noexcept -> decltype(auto);

	/**
	 * @brief Constant iterator to the first component.
	 *
	 * This creates a new constant iterator pointing to the first component of the vector.
	 */
	[[nodiscard]]
	constexpr auto cbegin(this auto const& self) noexcept -> decltype(auto);

	/**
	 * @brief Constant past-the-end iterator.
	 *
	 * This creates a new constant iterator pointing one past the end of the vector components.
	 */
	[[nodiscard]]
	constexpr auto cend(this auto const& self) noexcept -> decltype(auto);
};

template <std::size_t N, typename T>
vec(vec<N, T> const&) -> vec<N, T>;

template <std::size_t N, typename T>
vec(vec<N, T>&&) -> vec<N, T>;

template <typename... FromTs>
vec(FromTs const&... components) -> vec<sizeof...(FromTs), std::common_type_t<FromTs...>>;

/**
 * @brief Swaps two vectors.
 *
 * @tparam N dimension of vectors
 * @tparam T type of vectors
 *
 * @param lhs left-side vector
 * @param rhs right-side vector
 */
template <std::size_t N, typename T>
constexpr auto swap(vec<N, T>& lhs, vec<N, T>& rhs) noexcept -> void;

template <std::size_t N, typename T>
template <typename V>
struct vec<N, T>::iterator
{
	using difference_type = std::ptrdiff_t;
	using value_type      = vec<N, T>::value_type;
	using pointer         = std::add_pointer_t<value_type>;
	using reference       = std::add_lvalue_reference_t<value_type>;
	using const_reference = std::add_const_t<reference>;

	/**
	 * @brief Default constructor.
	 *
	 * Pointer to vector and position are value-initialized.
	 */
	constexpr iterator() noexcept = default;

	/**
	 * @brief Constructor from reference to vector and position.
	 *
	 * Initializes pointer to vector from address to @p v and position from @p pos.
	 */
	constexpr iterator(V& v, std::size_t pos) noexcept;

	/**
	 * @brief Indirection operator.
	 *
	 * Calls the subscript operator of stored vector with the argument equal to the value of stored position.
	 */
	[[nodiscard]]
	constexpr auto operator*(this auto&& self) -> decltype(auto);

	/**
	 * @brief Prefix increment operator.
	 *
	 * Increments the stored position.
	 */
	constexpr auto operator++(this iterator& self) noexcept -> iterator&;

	/**
	 * @brief Postfix increment operator.
	 *
	 * Increments the stored position of a copy of the iterator, then returns that copy.
	 */
	constexpr auto operator++(this iterator& self, int) noexcept -> iterator;

	/**
	 * @brief Prefix decrement operator.
	 *
	 * Decrements the stored position.
	 */
	constexpr auto operator--(this iterator& self) noexcept -> iterator&;

	/**
	 * @brief Postfix decrement operator.
	 *
	 * Decrements the stored position of a copy of the iterator, then returns that copy.
	 */
	constexpr auto operator--(this iterator& self, int) noexcept -> iterator;

	/**
	 * @brief Addition assignment operator.
	 *
	 * Adds @p inc to the stored position.
	 */
	constexpr auto operator+=(this iterator& self, difference_type inc) noexcept -> iterator&;

	/**
	 * @brief Subtraction assignment operator.
	 *
	 * Subtracts @p dec from the stored position.
	 */
	constexpr auto operator-=(this iterator& self, difference_type dec) noexcept -> iterator&;

	/**
	 * @brief Addition operator.
	 *
	 * Adds @p inc to the stored position of a copy of the iterator, then returns that copy.
	 */
	[[nodiscard]]
	constexpr auto operator+(this iterator const& self, difference_type inc) noexcept -> iterator;

	/**
	 * @brief Subtraction operator.
	 *
	 * Subtracts @p dec from the stored position of a copy of the iterator, then returns that copy.
	 */
	[[nodiscard]]
	constexpr auto operator-(this iterator const& self, difference_type dec) noexcept -> iterator;

	/**
	 * @brief Equality comparison operator.
	 *
	 * Compares stored pointer to vector and position of two iterators.
	 *
	 * @return @c true if two iterators are equal, @c false otherwise
	 */
	[[nodiscard]]
	constexpr auto operator==(this iterator const& self, iterator const& other) noexcept -> bool;

	/**
	 * @brief Inequality comparison operator.
	 *
	 * Compares stored pointer to vector and position of two iterators.
	 *
	 * @return @c false if two iterators are equal, @c true otherwise
	 */
	[[nodiscard]]
	constexpr auto operator!=(this iterator const& self, iterator const& other) noexcept -> bool;

private:
	/// Pointer to vector.
	V* v_{};

	/// Pointed to component index.
	std::size_t pos_{};
};
}

#include "vec.inl"

#endif
