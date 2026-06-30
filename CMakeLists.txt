cmake_minimum_required(VERSION 3.25)
project(SnazzCraft LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options()
else()
    add_compile_options(-Werror -Wall -Ofast)
endif()

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS   "src/*.c")
file(GLOB_RECURSE GLAD_SOURCE CONFIGURE_DEPENDS "includes/*glad.c")

add_executable(executable ${CPP_SOURCES} ${C_SOURCES} ${GLAD_SOURCE})

target_include_directories(executable PRIVATE includes)

# -> GLFW
add_subdirectory("dependencies/glfw")

if (TARGET glfw AND NOT MSVC)
    target_compile_options(glfw PRIVATE -w)
endif()
# -> GLFW

# -> Libnoise
cmake_policy(PUSH)
    cmake_policy(SET CMP0048 NEW) 
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    add_subdirectory("dependencies/libnoise")
cmake_policy(POP)

if(TARGET libnoise AND NOT MSVC)
    target_compile_options(libnoise PRIVATE -w)
endif()

target_link_libraries(executable PRIVATE
    OpenGL::GL
    glfw
    libnoise
)

target_include_directories(executable SYSTEM PRIVATE "dependencies/libnoise")
# <- Libnoise

set_target_properties(executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

add_custom_command(TARGET executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "../worlds"
)
