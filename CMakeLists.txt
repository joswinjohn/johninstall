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
    src/lib.cpp
)

target_include_directories(
    johninstall_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

target_compile_features(johninstall_lib PUBLIC cxx_std_17)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(johninstall_lib PRIVATE fmt::fmt)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
target_include_directories(johninstall_lib PRIVATE ${CURSES_INCLUDE_DIRS})
target_compile_options(johninstall_lib PRIVATE ${CURSES_CFLAGS})
target_link_libraries(johninstall_lib PRIVATE ${CURSES_LIBRARIES})

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(johninstall_lib PRIVATE nlohmann_json::nlohmann_json)

# ---- Declare executable ----

add_executable(johninstall_exe src/main.cpp)
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
