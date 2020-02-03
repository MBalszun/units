
add_executable(mba_units_example1
	example1.cpp
)

target_link_libraries(mba_units_example1 PRIVATE MBa::units)

add_test(NAME mba_units_example1 COMMAND mba_units_example1)


