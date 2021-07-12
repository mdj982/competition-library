#include "auto_util_header.hpp"

#if __cplusplus > 201703L
// C++20 or later
auto curry = [](auto f, auto&&... pres)
{
	return [f, ...pres = std::ref(pres)](auto&& ... args) mutable {
		return f(pres..., std::ref(args)...);
	};
};
#else // __cplusplus
// C++17 or later
auto curry = [](auto f, auto&&... pres)
{
	return [f, pres = std::make_tuple(std::ref(pres)...)](auto && ...args) mutable {
		return std::apply([&f](auto&& ... xs) {
			return f(xs...);
		}, std::tuple_cat(pres, std::make_tuple(std::ref(args)...)));
	};
};
#endif // __cplusplus
