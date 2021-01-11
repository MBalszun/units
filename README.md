# MBa Units

A small units library supporting intregral powers of kilogram, meter and seconds and - spearate from that - angles in rad

Here is a simple example, of how this can be used

	#include <mba-units/units.hpp>
	#include <mba-units/fmt.hpp>

	#include <iostream>

	using namespace mba;

	constexpr units::UPos distance_traveled( units::USpeed v, units::UAccel a, units::UTime t )
	{
		return t * v + 0.5 * a * square( t );
		// equivalent:
		// return t * v +  a * t * t / 2.0
	}

	using namespace mba::units::litterals;

	constexpr auto initial_speed = 2.0_mps;
	constexpr auto acceleration  = units::UAccel{1.0};

	int main()
	{

		constexpr auto dist = distance_traveled( initial_speed, acceleration, 3.0_s );
		static_assert( dist == 10.5_m );

		std::cout << "With an initial speed of " << sformat( initial_speed ) << " and an acceleration of "
				  << sformat( acceleration ) << " we will travel " << sformat( dist ) << " in " << sformat( 3.0_s )
				  << std::endl;

	/**
	 * Prints:
	 *
	 * With an initial speed of 2m_s^-1 and an acceleration of 1m_s^-2 we will travel 10.5m in 3s
	 *
	 */

	// Compile time error examples
	#if 0
		auto wrong_order = distance_traveled( acceleration, initial_speed, 3.0_s );
	#endif

	#if 0
		auto strange = acceleration + initial_speed;
	#endif
	}

