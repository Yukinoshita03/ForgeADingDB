add_executable(miniredbase_core_test core_test.cpp)
target_link_libraries(miniredbase_core_test PRIVATE miniredbase_core)

add_test(NAME miniredbase.core COMMAND miniredbase_core_test)
add_test(NAME miniredbase.version COMMAND miniredbase --version)

