#
# \file       CMakeLists.txt
# \author     GrandChris
# \date       2021-06-20
# \brief      Base project file
#

cmake_minimum_required(VERSION 3.20)

# Create the project
project("vulkan_erruption")

# Export compile commands for VSCode C++ Intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add subdirectories
add_subdirectory("app")
# add_subdirectory("source")

# link vcpkg packages
target_include_directories("app" PRIVATE vcpkg_installed/x64-mingw-static/include)
target_link_directories("app" PRIVATE vcpkg_installed/x64-mingw-static/lib)

target_link_libraries("app" PRIVATE glfw3)
