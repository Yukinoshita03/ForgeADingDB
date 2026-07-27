add_executable(forgeadingdb_core_test core_test.cpp)
target_link_libraries(forgeadingdb_core_test PRIVATE forgeadingdb_core)

add_test(NAME forgeadingdb.core COMMAND forgeadingdb_core_test)
add_test(NAME forgeadingdb.version COMMAND forgeadingdb --version)

