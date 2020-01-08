cmake_minimum_required(VERSION 3.7)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 777)
endif()

project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
set (bulk_files observer.h Math.h Math.cpp metrics.h metrics.cpp threadManager.h threadManager.cpp command_handler.h observer.cpp command_handler.cpp)

add_executable(bulkmt bulkmt.cpp ${bulk_files})

find_package(Threads REQUIRED)


add_executable(test_bulkmt_regular test_bulkmt_regular.cpp ${bulk_files})
add_executable(test_bulkmt_nested test_bulkmt_nested.cpp ${bulk_files})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set_target_properties(bulkmt test_bulkmt_regular test_bulkmt_nested PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(test_bulkmt_regular test_bulkmt_nested PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(bulkmt
    Threads::Threads
)

target_link_libraries(test_bulkmt_regular
    Threads::Threads
    ${Boost_LIBRARIES}
)

target_link_libraries(test_bulkmt_nested
    Threads::Threads
    ${Boost_LIBRARIES}
)

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT "SergeiNA at github.com")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(regular_tests test_bulkmt_regular)
add_test(nested_tests test_bulkmt_nested)