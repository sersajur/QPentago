cmake_minimum_required(VERSION 3.10)
project(GoogleTest CXX)

# Warning fixes - borrowed from GTest build scripts
if(WIN32)
  set(gtest_cxx_flags -D_UNICODE -DUNICODE -DSTRICT -DWIN32_LEAN_AND_MEAN)
else()
  set(gtest_cxx_flags -Wno-unused-parameter -Wno-missing-field-initializers)
endif()

# gtest

add_library(gtest STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest-all.cc"
)

target_include_directories(gtest
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
)

target_compile_options(gtest
  PRIVATE ${gtest_cxx_flags}
)

if (UNIX)
  target_link_libraries(gtest
    PRIVATE pthread
  )
endif()

# gtest_main

add_library(gtest_main STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/src/gtest_main.cc"
)

target_include_directories(gtest_main
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
)

target_compile_options(gtest_main
  PRIVATE ${gtest_cxx_flags}
)

#
