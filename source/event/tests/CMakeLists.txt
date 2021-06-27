set(EXE_NAME "event_tests")

add_executable(${EXE_NAME})

set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)

# target_compile_options(${EXE_NAME} PRIVATE
#   -fcoroutines 

#   -fdiagnostics-color
#   -Wall 

#   -fsanitize=address 
#   -fsanitize=undefined
#   -fsanitize=leak

#   $<$<CONFIG:Debug>:-g -O0>
#   $<$<CONFIG:Release>:-g -O3>
# )

target_include_directories(${EXE_NAME} PRIVATE ../)

# Add source files
file(GLOB SOURCE_FILES_TEST CONFIGURE_DEPENDS
    "./*.cpp"
)

target_sources(${EXE_NAME} PRIVATE ${SOURCE_FILES_TEST})

# Add cppcoro
# target_link_libraries(${EXE_NAME} PRIVATE 
#   ${PROJECT_NAME}
#   ${LIBASAN}
#   ${LIBLSAN}
#   ${LIBUBSAN}
# )

# Add GTest
# find_package(GTest CONFIG REQUIRED)

target_link_libraries(${EXE_NAME} PRIVATE gtest gtest_main)

# enable_testing()
# include(GoogleTest)
# gtest_discover_tests(${PROJECT_TEST_NAME})