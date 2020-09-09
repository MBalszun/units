#include <mba-units/units.hpp>

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
constexpr void check_comparisons( Ut )
{
	constexpr Ut u1{1.0};
	constexpr Ut u2{1.1};

	static_assert( u1 < u2 );
	static_assert( u1 <= u2 );
	static_assert( !( u1 > u2 ) );
	static_assert( !( u1 >= u2 ) );
	static_assert( !( u1 == u2 ) );
	static_assert( u1 != u2 );

	static_assert( !( u2 < u1 ) );
	static_assert( !( u2 <= u1 ) );
	static_assert( u2 > u1 );
	static_assert( u2 >= u1 );
	static_assert( !( u2 == u1 ) );
	static_assert( u2 != u1 );

	static_assert( !( u1 < u1 ) );
	static_assert( u1 <= u1 );
	static_assert( !( u1 > u1 ) );
	static_assert( u1 >= u1 );
	static_assert( u1 == u1 );
	static_assert( !( u1 != u1 ) );
}

template<class Ut>
constexpr void check_add_sub( Ut )
{
	constexpr Ut u1{8.0};
	constexpr Ut u2{-16.0};

	static_assert( u1 + u2 == -Ut{8.0} );
	static_assert( u1 - u2 == +Ut{24.0} );
}

template<class Ut>
constexpr void check_implicit_conversion( Ut )
{
	static_assert( std::is_same_v<Ut, units::UNone>
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
	check_comparisons( u );
	check_add_sub( u );
	return 0;
}

constexpr void check_imlicit_initialization()
{
	constexpr units::UNone unit1 = 5.0;
	static_assert( unit1.value == 5.0 );

	static_assert( std::is_convertible_v<units::UNone, double> );
}

constexpr auto check_dimless_ops()
{
	check_imlicit_initialization();
	check_assignment<units::UNone>( 0.1 );
	check_assignment<units::UNone>( units::UNone{1.0} );
	return 1;
}

template<class U_l, class U_r>
constexpr bool check_multiplication( U_l, U_r )
{
	using r_t = units::UMultiply_t<U_l, U_r>;

	static_assert( U_l{2.0} * U_r{-4.0} == r_t{-8.0} );
	static_assert( U_l{-8.0} * U_r{0.25} == r_t{-2.0} );
	return true;
}

template<class U_l, class U_r>
constexpr bool check_division( U_l, U_r )
{
	using r_t = units::UDivide_t<U_l, U_r>;

	static_assert( U_l{2.0} / U_r{-4.0} == r_t{-0.5} );
	static_assert( U_l{-8.0} / U_r{0.25} == r_t{-32.0} );
	return true;
}

template<class U_l, class U_r>
constexpr auto check_combination( U_l l, U_r r )
{
	check_multiplication( l, r );
	check_division( l, r );
	return 1;
}

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

/* Helpers */
constexpr int check_canTakeSqrt()
{
	using namespace mba::units::litterals;
	static_assert( units::canTakeSqrt<0, 0, 0>( {} ) == true );
	static_assert( units::canTakeSqrt<0, 0, 0>() == true );
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

	static_assert( units::canTakeSqrt( 1.25_kg ) == false );
	static_assert( units::canTakeSqrt( 1.25_kg * 1.25_kg ) == true );

	static_assert( units::canTakeSqrt( 1.0_mps2 ) == false );
	static_assert( units::canTakeSqrt( 1.0_s * 1.0_hz ) == true );

	return 1;
}

[[maybe_unused]] constexpr auto gc1 = check_common_ops_for( units::Unit<1, 2, 3>{1.0} );
[[maybe_unused]] constexpr auto gc2 = check_common_ops_for( units::Unit<1, 2, -3>{1.0} );
[[maybe_unused]] constexpr auto gc3 = check_common_ops_for( units::Unit<0, 0, 0>{1.0} );
[[maybe_unused]] constexpr auto gc4 = check_common_ops_for( units::Unit<-1000, 5, 70>{1.0} );
[[maybe_unused]] constexpr auto gc5 = check_common_ops_for( units::UAngle{1.0} );

[[maybe_unused]] constexpr auto dlc1 = check_dimless_ops();

[[maybe_unused]] constexpr auto coc1 = check_combination( units::Unit<1, 2, 3>{1.0}, 1.0 );
[[maybe_unused]] constexpr auto coc2 = check_combination( 1.0, units::Unit<1, 2, 3>{1.0} );
[[maybe_unused]] constexpr auto coc3 = check_combination( units::UNone{}, units::Unit<1, 2, 3>{1.0} );
[[maybe_unused]] constexpr auto coc4 = check_combination( units::Unit<1, 2, 3>{}, units::UNone{} );
[[maybe_unused]] constexpr auto coc5 = check_combination( units::UNone{}, units::UNone{} );
[[maybe_unused]] constexpr auto coc6 = check_combination( units::Unit<1, 2, 3>{}, units::Unit<-1, -2, -3>{} );

[[maybe_unused]] constexpr auto coc7 = check_division( units::UAngle{}, units::UAngle{} );
[[maybe_unused]] constexpr auto coc8 = check_division( units::UAngle{}, double{} );
[[maybe_unused]] constexpr auto coc9 = check_multiplication( units::UAngle{}, double{} );
[[maybe_unused]] constexpr auto coc10 = check_multiplication( double{}, units::UAngle{} );


[[maybe_unused]] constexpr auto pc1 = check_litterals();

[[maybe_unused]] constexpr auto hc2 = check_canTakeSqrt();

} // namespace


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

int main()
{
	return 0;
}