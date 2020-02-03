#include <mba-units/interop_chrono.hpp>



namespace {

constexpr auto cres1 = mba::units::from_std_duration( std::chrono::milliseconds( 250 ) );

static_assert( cres1 == mba::units::UTime( 0.25 ) );

constexpr auto cres2 = mba::units::from_std_duration( std::chrono::seconds( 250 ) );

static_assert( cres2 == mba::units::UTime( 250 ) );

}
