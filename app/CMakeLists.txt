#
# \file       CMakeLists.txt
# \author     GrandChris
# \date       2021-06-20
# \brief      Simple test application
#

set(APP_NAME app)
add_executable(${APP_NAME})

file(GLOB SOURCE_FILES CONFIGURE_DEPENDS
    "*.cpp"
)

target_sources(${APP_NAME} PRIVATE ${SOURCE_FILES})

target_link_libraries(${APP_NAME} PRIVATE vulkan_erruption)