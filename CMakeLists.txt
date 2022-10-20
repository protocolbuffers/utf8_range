cmake_minimum_required (VERSION 3.5)
project (utf8_range C CXX)

# option() honor variables
if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)

option (utf8_range_ENABLE_TESTS "Build test suite" ON)
option (utf8_range_ENABLE_INSTALL "Configure installation" ON)

##
# Create the lightweight C library
add_library (utf8_range STATIC
  naive.c
  range2-neon.c
  range2-sse.c
)
target_sources (utf8_range PUBLIC FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
  FILES utf8_range.h
)

##
# A heavier-weight C++ wrapper that supports Abseil.
add_library (utf8_validity STATIC utf8_validity.cc)
target_sources (utf8_validity PUBLIC FILE_SET HEADERS
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
  FILES utf8_validity.h
)

# Load Abseil dependency.
if (NOT ABSL_ROOT_DIR)
  find_package(absl REQUIRED CONFIG)
endif ()
if (NOT TARGET absl::strings)
  set(ABSL_ENABLE_INSTALL ${utf8_range_ENABLE_INSTALL})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(${ABSL_ROOT_DIR} third_party/abseil-cpp)
endif ()
target_link_libraries(utf8_validity PUBLIC absl::strings)

# Configure tests.
if (utf8_range_ENABLE_TESTS)
  enable_testing()

  find_package(GTest REQUIRED)

  add_executable(tests utf8_validity_test.cc)
  target_link_libraries(tests utf8_validity GTest::gmock_main)

  add_test(NAME utf8_validity_test COMMAND tests)

  add_custom_target(check
    COMMAND tests
    DEPENDS tests
  )
endif ()

# Configure installation.
if (utf8_range_ENABLE_INSTALL)
  install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
  install(TARGETS utf8_validity utf8_range EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS
  )
endif ()
