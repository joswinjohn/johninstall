cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    johninstall
    VERSION 0.1.0
    DESCRIPTION "JohnOS guided installer TUI"
    HOMEPAGE_URL "https://joswinjohn.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    johninstall_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    johninstall_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(johninstall_lib PUBLIC cxx_std_17)

find_package(fmt REQUIRED)
target_link_libraries(johninstall_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(johninstall_exe source/main.cpp)
add_executable(johninstall::exe ALIAS johninstall_exe)

set_property(TARGET johninstall_exe PROPERTY OUTPUT_NAME johninstall)

target_compile_features(johninstall_exe PRIVATE cxx_std_17)

target_link_libraries(johninstall_exe PRIVATE johninstall_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT johninstall_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of johninstall"
  )
endif()

include(cmake/dev-mode.cmake)
