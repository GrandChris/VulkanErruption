#
# \file       CMakeLists.txt
# \author     GrandChris
# \date       2021-06-20
# \brief      Base project file
#

cmake_minimum_required(VERSION 3.20)

# Create the project
project("vulkan_erruption" CXX)

# Export compile commands for VSCode C++ Intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    -fdiagnostics-color
    -Wall 
)




# add library
add_library(${PROJECT_NAME})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    source
)

# add source files
file(GLOB SOURCE_FILES CONFIGURE_DEPENDS
    # "source/vulkan_erruption/details/*.cpp"
    "source/vulkan_erruption/*.cpp"
    "source/vulkan_erruption/*/*.cpp"
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})



# link vcpkg packages
# target_include_directories(${PROJECT_NAME} PRIVATE vcpkg_installed/x64-mingw-static/include)
# target_link_directories(${PROJECT_NAME} PUBLIC vcpkg_installed/x64-mingw-static/lib)

include_directories(vcpkg_installed/x64-mingw-static/include)
link_directories(vcpkg_installed/x64-mingw-static/lib)
link_directories(vcpkg_installed/x64-mingw-static/lib/manual-link)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)


# add vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)


# Vulkan::glslc
set(FILE_TO_HEADER ${CMAKE_SOURCE_DIR}/shaders/FileToHeader.exe)

file(GLOB SHADER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/shaders/source/*"
)

foreach(SHADER_SOURCE ${SHADER_SOURCES})
    get_filename_component(file_c ${SHADER_SOURCE} NAME)
    # set(SHADER_SOURCE ${CMAKE_SOURCE_DIR}/shaders/source/shader.frag)${file_c}
    set(SHADER_SPV ${CMAKE_BINARY_DIR}/shaders/generated/${file_c}.spv)
    set(SHADER_HEADER ${CMAKE_BINARY_DIR}/shaders/generated/${file_c}.h)
    add_custom_command(
        OUTPUT ${SHADER_HEADER}
        DEPENDS ${SHADER_SOURCE}
        COMMAND Vulkan::glslc ${SHADER_SOURCE} -O -o ${SHADER_SPV}
        COMMAND ${FILE_TO_HEADER} ${SHADER_SPV} ${SHADER_HEADER}
    )
endforeach()


target_sources(${PROJECT_NAME} PRIVATE ${SHADER_HEADER})



# Add subdirectories
add_subdirectory("app")
add_subdirectory("source/event/tests")



