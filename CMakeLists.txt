cmake_minimum_required( VERSION 3.9 )
project( MBaUnits LANGUAGES CXX CXX VERSION 0.5 )

get_directory_property( hasParent PARENT_DIRECTORY )
if( NOT hasParent )
	set( MBA_UNITS_INCLUDE_TESTS_DEFAULT ${ENABLE_TESTING})
else()
	set( MBA_UNITS_INCLUDE_TESTS_DEFAULT OFF)
endif()

option( MBA_UNITS_INCLUDE_TESTS ${MBA_UNITS_INCLUDE_TESTS_DEFAULT} )

add_library( mba_units INTERFACE )
add_library( MBa::units ALIAS mba_units )
set_target_properties( mba_units PROPERTIES EXPORT_NAME units )

target_include_directories( mba_units
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

include(CTest)
if( MBA_UNITS_INCLUDE_TESTS )
	add_subdirectory( tests )
endif()

if( MBA_UNITS_INCLUDE_EXAMPLES OR MBA_UNITS_INCLUDE_TESTS )
	add_subdirectory( examples )
endif()
