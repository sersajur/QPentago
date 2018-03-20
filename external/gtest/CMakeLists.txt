# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
  gtest
  URL https://github.com/google/googletest/archive/master.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
  # Make gtest link C runtimes dynamically, otherwise our unittest target will fail linking gtest.
  CMAKE_CACHE_ARGS -DGTEST_FORCE_SHARED_CRT:BOOL=ON
  # Disable install step
  INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED UNKNOWN GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/googletest/include"
    IMPORTED_LOCATION_RELEASE "${binary_dir}/googlemock/gtest/Release/gtest.lib"
    IMPORTED_LOCATION_DEBUG "${binary_dir}/googlemock/gtest/Debug/gtestd.lib"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED UNKNOWN GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock
  PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/googlemock/include"
    IMPORTED_LOCATION_RELEASE "${binary_dir}/googlemock/Release/gmock.lib"
    IMPORTED_LOCATION_DEBUG "${binary_dir}/googlemock/Debug/gmockd.lib"
    IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
)
