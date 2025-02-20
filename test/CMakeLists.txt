# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(johninstallTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(johninstall_test source/johninstall_test.cpp)
target_link_libraries(
    johninstall_test PRIVATE
    johninstall_lib
    Catch2::Catch2WithMain
)
target_compile_features(johninstall_test PRIVATE cxx_std_17)

catch_discover_tests(johninstall_test)

# ---- End-of-file commands ----

add_folders(Test)
