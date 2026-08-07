module;

#include "deps.def"

export module ndml.tests:meta;

import ndml.meta;
import std;

namespace
{
using namespace ndml::meta;

template <typename Assignment>
auto check_assignment(Assignment const& assignment, int initial, int rhs, int expected) -> void
{
	auto  lhs    = initial;
	auto& result = assignment(lhs, rhs);

	REQUIRE(&result == &lhs);
	REQUIRE(lhs == expected);
}

TEST_CASE("burn types are empty distinct construction sinks", "[meta][burn]")
{
	using first  = burn_t<struct first_tag>;
	using second = burn_t<struct second_tag>;

	STATIC_REQUIRE(std::is_empty_v<first>);
	STATIC_REQUIRE(std::constructible_from<first>);
	STATIC_REQUIRE(std::constructible_from<first, int, double>);
	STATIC_REQUIRE(!std::same_as<first, second>);
	STATIC_REQUIRE(!std::is_assignable_v<first&, first const&>);
	STATIC_REQUIRE(!std::is_assignable_v<first&, first&&>);

	[[maybe_unused]] auto sink = first{1, 2.0};
}

TEST_CASE("arithmetic assignment functors apply and return the left operand", "[meta][functional]")
{
	check_assignment(copy_assignment<int>{}, 1, 7, 7);
	check_assignment(addition_assignment<int>{}, 4, 3, 7);
	check_assignment(subtraction_assignment<int>{}, 10, 3, 7);
	check_assignment(multiplication_assignment<int>{}, 6, 7, 42);
	check_assignment(division_assignment<int>{}, 42, 6, 7);
	check_assignment(remainder_assignment<int>{}, 17, 5, 2);
}

TEST_CASE("bitwise assignment functors apply and return the left operand", "[meta][functional]")
{
	check_assignment(bit_and_assignment<int>{}, 0b1100, 0b1010, 0b1000);
	check_assignment(bit_or_assignment<int>{}, 0b1100, 0b0011, 0b1111);
	check_assignment(bit_xor_assignment<int>{}, 0b1100, 0b1010, 0b0110);
	check_assignment(left_shift_assignment<int>{}, 3, 2, 12);
	check_assignment(right_shift_assignment<int>{}, 12, 2, 3);
}
}
