#pragma once

#include "./units.hpp"

#include <chrono>

namespace mba::units{

	template<class Rep, class Period>
	constexpr UTime from_std_duration( std::chrono::duration<Rep, Period> d )
	{
		using std_float_dur_type = std::chrono::duration<double, std::ratio<1>>;
		return UTime{std::chrono::duration_cast<std_float_dur_type>( d ).count()};
	}
}