#include <mba-units/units.hpp>

#include <type_traits>
#pragma once

#include <chrono>
#include <cmath>
#include <type_traits>

using namespace mba;
namespace {
constexpr units::UGen gen{0.1};

template<class Ut>
constexpr void check_common_initialization( Ut )
{
	constexpr Ut unit1;
	static_assert( unit1.value == 0.0 );

	constexpr Ut unit2{};
	static_assert( unit2.value == 0.0 );

	constexpr Ut unit3{gen};
	static_assert( unit3.value == 0.1 );

	constexpr Ut unit4( gen );
	static_assert( unit4.value == 0.1 );

	constexpr Ut unit5{0.1};
	static_assert( unit5.value == 0.1 );

	constexpr Ut unit6( 5.5 );
	static_assert( unit6.value == 5.5 );
}

template<class Ut, class SUt>
constexpr void check_assignment( SUt )
{
	auto ass = []( auto v ) {
		Ut u{};
		u = v;
		return u;
	};
	static_assert( SUt{ass( SUt{1.0} ).value} == SUt{1.0} );
	static_assert( SUt{ass( SUt{-1.0} ).value} == SUt{-1.0} );
	static_assert( SUt{ass( SUt{0.0} ).value} == SUt{0.0} );
}

template<class Ut>
constexpr void check_implicit_conversion( Ut )
{
	static_assert( std::is_same_v<Ut, units::Unit<0, 0, 0>>
					   ? std::is_convertible_v<Ut, double> && std::is_convertible_v<double, Ut>
					   : !std::is_convertible_v<Ut, double> && !std::is_convertible_v<double, Ut> );
}

constexpr auto double_add_18_and_div_by_16_compount_ops = []( auto base ) {
	using U_t = decltype( base );
	U_t u1{2.0};
	U_t u2{4.0};
	U_t u3{-4.0};
	base *= 2.0;
	base += u1;
	base += u2;
	base -= u3;
	base += U_t{8.0};
	base /= 16.0;

	return base;
};

template<class Ut>
constexpr void check_compund_ops( Ut )
{
	constexpr Ut u{2.0};
	static_assert( double_add_18_and_div_by_16_compount_ops( u ) == Ut{( u.value * 2.0 + 18.0 ) / 16.0} );
}

template<class Ut>
constexpr auto check_common_ops_for( Ut u )
{
	check_common_initialization( u );
	check_assignment<Ut>( u );
	check_implicit_conversion( u );
	check_compund_ops( u );
	return 0;
}

[[maybe_unused]] constexpr auto gc1 = check_common_ops_for( units::Unit<1, 2, 3>{1.0} );
[[maybe_unused]] constexpr auto gc2 = check_common_ops_for( units::Unit<1, 2, -3>{1.0} );
[[maybe_unused]] constexpr auto gc3 = check_common_ops_for( units::Unit<0, 0, 0>{1.0} );
[[maybe_unused]] constexpr auto gc4 = check_common_ops_for( units::Unit<-1000, 5, 70>{1.0} );

constexpr void check_imlicit_initialization()
{
	constexpr units::Unit<0, 0, 0> unit1 = 5.0;
	static_assert( unit1.value == 5.0 );

	static_assert( std::is_convertible_v<units::Unit<0, 0, 0>, double> );
}

constexpr auto check_dimless_ops()
{
	check_imlicit_initialization();
	check_assignment<units::UNone>( 0.1 );
	check_assignment<units::UNone>( units::Unit<0, 0, 0>{1.0} );
	return 1;
}

[[maybe_unused]] constexpr auto dlc1 = check_dimless_ops();

constexpr auto check_litterals()
{
	using namespace mba::units::litterals;
	static_assert( units::UMass{1.25} == 1.25_kg );
	static_assert( units::UPos{1.25} == 1.25_m );
	static_assert( units::UTime{1.25} == 1.25_s );
	static_assert( units::UHerz{1.25} == 1.25_hz );
	static_assert( units::USpeed{1.25} == 1.25_mps );
	static_assert( units::UAccel{1.25} == 1.25_mps2 );
	static_assert( units::UForce{1.25} == 1.25_n );
	return 1;
}

[[maybe_unused]] constexpr auto pc1 = check_litterals();

/* Helpers */
constexpr int check_canTakeSqrt()
{
	using namespace mba::units::litterals;
	static_assert( units::canTakeSqrt<0, 0, 0>( {} ) == true );
	static_assert( units::canTakeSqrt<0, 0, 0>( ) == true );
	static_assert( units::canTakeSqrt<1, 2, 2>() == false );
	static_assert( units::canTakeSqrt<2, 2, 2>() == true );
	static_assert( units::canTakeSqrt<-2, -2, -2>() == true );

	static_assert( units::canTakeSqrt<6, 6, 6>() == true );

	static_assert( units::canTakeSqrt<5, 6, 6>() == false );
	static_assert( units::canTakeSqrt<6, 5, 6>() == false );
	static_assert( units::canTakeSqrt<6, 6, 5>() == false );

	static_assert( units::canTakeSqrt<5, 5, 6>() == false );
	static_assert( units::canTakeSqrt<5, 6, 5>() == false );
	static_assert( units::canTakeSqrt<6, 5, 5>() == false );

	static_assert( units::canTakeSqrt<5, 5, 5>() == false );

	static_assert( units::canTakeSqrt( 1.25_kg  ) == false );
	static_assert( units::canTakeSqrt(1.25_kg * 1.25_kg) == true );

	static_assert( units::canTakeSqrt( 1.0_mps2 ) == false );
	static_assert( units::canTakeSqrt( 1.0_s * 1.0_hz ) == true );

	return 1;
}

[[maybe_unused]] constexpr auto hc2 = check_canTakeSqrt();

} // namespace

// namespace mba::units {
//
//
// template<class Rep, class Period>
// constexpr UTime from_std_duration( std::chrono::duration<Rep, Period> d )
//{
//	using std_float_dur_type = std::chrono::duration<double, std::ratio<1>>;
//	return UTime{std::chrono::duration_cast<std_float_dur_type>( d ).count()};
//}
//
//} // namespace default_unit_definitions
//
////##### Operator overload for Unit #####
//// operators, where the units can be different and the result (may) be another type yet again
// template<int k1, int m1, int s1, int k2, int m2, int s2>
// constexpr Unit<k1 + k2, m1 + m2, s1 + s2> operator*( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r )
//{
//	return Unit<k1 + k2, m1 + m2, s1 + s2>{l.value * r.value};
//};
//
// template<int k1, int m1, int s1, int k2, int m2, int s2>
// constexpr Unit<k1 - k2, m1 - m2, s1 - s2> operator/( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r )
//{
//	return Unit<k1 - k2, m1 - m2, s1 - s2>{l.value / r.value};
//};
//
//// operations, where both operators have to be of the same type
// template<int k, int m, int s>
// constexpr auto operator+( Unit<k, m, s> l, Unit<k, m, s> r ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{l.value + r.value};
//}
//
// template<int k, int m, int s>
// constexpr auto operator-( Unit<k, m, s> l, Unit<k, m, s> r ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{l.value - r.value};
//}
//
//// operations with scalars
// template<int k, int m, int s>
// constexpr auto operator*( Unit<k, m, s> l, double r ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{l.value * r};
//}
//
// template<int k, int m, int s>
// constexpr auto operator*( double l, Unit<k, m, s> r ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{l * r.value};
//}
//
// template<int k, int m, int s>
// constexpr auto operator/( Unit<k, m, s> l, double r ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{l.value / r};
//}
//
// template<int k, int m, int s>
// constexpr auto operator/( double l, Unit<k, m, s> r ) -> Unit<-k, -m, -s>
//{
//	return Unit<-k, -m, -s>{l / r.value};
//}
//
//// unary operators
// template<int k, int m, int s>
// constexpr auto operator-( Unit<k, m, s> l ) -> Unit<k, m, s>
//{
//	return Unit<k, m, s>{-l.value};
//}
//
// template<int k, int m, int s>
// constexpr auto operator+( Unit<k, m, s> l ) -> Unit<k, m, s>
//{
//	return l;
//}
//
//// relational operators
// template<int k, int m, int s>
// constexpr bool operator<( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value < r.value};
//}
//
// template<int k, int m, int s>
// constexpr bool operator>( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value > r.value};
//}
//
// template<int k, int m, int s>
// constexpr bool operator==( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value == r.value};
//}
//
// template<int k, int m, int s>
// constexpr bool operator!=( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value != r.value};
//}
//
// template<int k, int m, int s>
// constexpr bool operator<=( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value <= r.value};
//}
//
// template<int k, int m, int s>
// constexpr bool operator>=( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return {l.value >= r.value};
//}
//
//// helper types to get the result of a mathematical operation on units
// namespace _unit_impl {
//
// template<class U1, class U2>
// struct UDivide {
//};
//
// template<class U1, class U2>
// struct UMultiply {
//};
//
// template<int k1, int m1, int s1, int k2, int m2, int s2>
// struct UDivide<Unit<k1, m1, s1>, Unit<k2, m2, s2>> {
//	using type = Unit<k1 - k2, m1 - m2, s1 - s2>;
//};
//
// template<int k1, int m1, int s1, int k2, int m2, int s2>
// struct UMultiply<Unit<k1, m1, s1>, Unit<k2, m2, s2>> {
//	using type = Unit<k1 + k2, m1 + m2, s1 + s2>;
//};
//
//} // namespace _unit_impl
//
// template<class U1, class U2>
// using UDivide_t = typename _unit_impl::UDivide<U1, U2>::type;
//
// template<class U1, class U2>
// using UMultiply_t = typename _unit_impl::UMultiply<U1, U2>::type;
//
// template<class U1>
// using UInverse_t = UDivide_t<UNone, U1>;
//
//// more complex mathematical operations
//
//// c++14: constexpr auto sqrt(Unit<k, m, s> l) -> std::enable_if_t < canTakeSqrt(Unit<k, m, s>{}), Unit < (k / 2), (m
//// / 2), (s / 2) >> ::type{
// template<int k, int m, int s>
// constexpr auto sqrt( Unit<k, m, s> l ) -> Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>
//{
//	static_assert( canTakeSqrt( Unit<k, m, s>{} ), "Base units are not a power of 2" );
//	return Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>( std::sqrt( l.value ) );
//}
//
// template<int k, int m, int s>
// constexpr auto square( Unit<k, m, s> l ) -> Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>
//{
//	return Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>( l.value * l.value );
//}
//
// template<int k, int m, int s>
// constexpr auto abs( Unit<k, m, s> l )
//{
//	return Unit<k, m, s>( std::abs( l.value ) );
//}
//
// template<int k, int m, int s>
// constexpr auto max( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return Unit<k, m, s>( l.value > r.value ? l.value : r.value );
//}
//
// template<int k, int m, int s>
// constexpr auto min( Unit<k, m, s> l, Unit<k, m, s> r )
//{
//	return Unit<k, m, s>( l.value < r.value ? l.value : r.value );
//}
//
////##### Operator overload for Angle #####
//
// namespace _detail_angle {
//
// constexpr long double pi_internal = 3.141592653589793238462643383279502884;
// constexpr double      pi          = static_cast<double>( pi_internal );
// constexpr double      pi2         = static_cast<double>( 2 * pi_internal );
//
// constexpr auto modulo2Pi( const double& a )
//{
//	return a - ( pi2 * static_cast<long long>( a / pi2 ) );
//}
//
// constexpr double normNegPiPi( double angle )
//{
//	// TODO: use c++14 constexpr syntax
//	return angle < -pi ? normNegPiPi( angle + pi2 ) : angle > pi ? normNegPiPi( angle - pi2 ) : angle;
//}
// constexpr double normNeg2Pi2Pi( double angle )
//{
//	// TODO: use c++14 constexpr syntax
//	return normNegPiPi( modulo2Pi( angle ) );
//}
//
//} // namespace _detail_angle
//
// struct UAngle {
//	double value;
//	constexpr UAngle()
//		: value{}
//	{
//	}
//	constexpr explicit UAngle( double v )
//		: value( v ){};
//	constexpr explicit UAngle( Unit<0, 0, 0> v )
//		: value( v.value ){};
//	constexpr operator Unit<0, 0, 0>() const { return value; }
//
//	UAngle& operator+=( UAngle other )
//	{
//		value += other.value;
//		return *this;
//	}
//	UAngle& operator-=( UAngle other )
//	{
//		value += other.value;
//		return *this;
//	}
//	UAngle& operator*=( double other )
//	{
//		value *= other;
//		return *this;
//	}
//	UAngle& operator/=( double other )
//	{
//		value /= other;
//		return *this;
//	}
//};
//
// constexpr UAngle pi = static_cast<UAngle>( (double)_detail_angle::pi_internal );
//
// constexpr UAngle normNegPiPi( UAngle angle )
//{
//	return UAngle{_detail_angle::normNegPiPi( angle.value )};
//}
//
// constexpr UAngle normNeg2Pi2Pi( UAngle angle )
//{
//	return UAngle{_detail_angle::normNeg2Pi2Pi( angle.value )};
//}
//
//// clang-format off
//// binary math operators
// constexpr auto operator*( UAngle l, double r ) { return UAngle{l.value * r}; }
// constexpr auto operator*( double l, UAngle r ) { return UAngle{l * r.value}; }
//
// constexpr auto operator/( UAngle l, UAngle r ) { return l.value / r.value;	 }
// constexpr auto operator/( UAngle l, double r ) { return UAngle{l.value / r}; }
// constexpr auto operator/( double l, UAngle r ) { return UAngle{l / r.value}; }
//
// constexpr auto operator+( UAngle l, UAngle r ){	return UAngle{l.value + r.value}; }
// constexpr auto operator-( UAngle l, UAngle r ){	return UAngle{l.value - r.value}; }
//
//// unary math operators
// constexpr auto operator-( UAngle angle ){ return UAngle{-angle.value}; }
// constexpr auto operator+( UAngle angle ){ return angle; }
//
//// comparison operators
// constexpr bool operator< ( UAngle l, UAngle r ) { return l.value < r.value;  }
// constexpr bool operator> ( UAngle l, UAngle r ) { return l.value > r.value;	 }
// constexpr bool operator==( UAngle l, UAngle r ) { return l.value == r.value; }
// constexpr bool operator!=( UAngle l, UAngle r ) { return l.value != r.value; }
// constexpr bool operator<=( UAngle l, UAngle r ) { return l.value <= r.value; }
// constexpr bool operator>=( UAngle l, UAngle r )	{ return l.value >= r.value; }
//
//// clang-format on
//
// inline auto abs( UAngle l )
//{
//	return UAngle( std::abs( l.value ) );
//}
//
// inline auto cos( UAngle l )
//{
//	return std::cos( l.value );
//}
//
// inline auto sin( UAngle l )
//{
//	return std::sin( l.value );
//}
//
// inline UAngle atan2( UPos Y, UPos X )
//{
//	return UAngle{std::atan2( Y.value, X.value )};
//}
//
//} // namespace mba::units

int main() {}