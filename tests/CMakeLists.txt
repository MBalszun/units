
add_executable(mba_units_tests
	test_units.cpp
	test_chrono_interop.cpp
	test_fmt.cpp
)

target_link_libraries(mba_units_tests PRIVATE MBa::units)

add_test(NAME mba_tests_units_main COMMAND mba_units_tests)


