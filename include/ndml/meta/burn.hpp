#ifndef NDML_META_BURN_HPP
#define NDML_META_BURN_HPP

namespace ndml::meta
{
/**
 * @brief Burn type.
 *
 * An instance of this type can be constructed from any parameters, but cannot be assigned to.
 * It is used, for example, in @c ndml::vec for representing unusable components.
 */
template <typename>
struct burn_t
{
	/**
	 * @brief Constructor from any parameters.
	 *
	 * It can be used to initialize an instance of burn type with any parameters.
	 *
	 * @tparam Ts types of parameters
	 */
	template <typename... Ts>
	constexpr burn_t(Ts const&...) noexcept;

	/**
	 * @brief Assignment operator.
	 *
	 * It is deleted to disallow assignments.
	 *
	 * @tparam T parameter type
	 */
	template <typename T>
	constexpr auto operator=(T) noexcept -> burn_t& = delete;
};
}

#include "burn.inl"

#endif
