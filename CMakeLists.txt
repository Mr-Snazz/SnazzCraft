cmake_minimum_required(VERSION 3.25)
project(VoxelEngine LANGUAGES C CXX)

# 1. Language Standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. Compiler Flags (-Wall -Werror equivalent)
if(MSVC)
    add_compile_options(/W4 /WX) # Windows strict flags
else()
    add_compile_options(-Wall -Werror) # Linux/macOS strict flags
endif()

# 3. Find Core Packages
find_package(OpenGL REQUIRED)

# Try to find GLFW natively via CMake configuration
find_package(glfw3 CONFIG QUIET)
if(NOT glfw3_FOUND)
    find_package(GLFW3 QUIET)
endif()

# 4. Source Files Discovery
# CONFIGURE_DEPENDS forces CMake to check for new files automatically when building
# CORRECT ORDER: Sub-command -> Options -> Flags -> Patterns
file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE GLAD_SOURCE CONFIGURE_DEPENDS "includes/*glad.c")

# 5. Define the Executable
add_executable(executable ${CPP_SOURCES} ${C_SOURCES} ${GLAD_SOURCE})

# 6. Include Directories
target_include_directories(executable PRIVATE includes)

# 7. Link Libraries (Cross-Platform mapping)
target_link_libraries(executable PRIVATE OpenGL::GL)

# Link GLFW depending on how CMake found it
if(TARGET glfw)
    target_link_libraries(executable PRIVATE glfw)
elseif(TARGET glfw3)
    target_link_libraries(executable PRIVATE glfw3)
else()
    target_link_libraries(executable PRIVATE glfw) # Fallback string matching
endif()

# Platform-specific library linking (dl, m, noise)
if(UNIX AND NOT APPLE)
    # Linux specific libraries
    target_link_libraries(executable PRIVATE dl m noise)
elseif(WIN32)
    # Windows doesn't use 'dl' or 'm'. It just needs 'noise'
    target_link_libraries(executable PRIVATE noise)
endif()

# 8. Match your old 'bin/' output layout
set_target_properties(executable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# This tells CMake to create the folder as part of building your executable
add_custom_command(TARGET executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "../worlds"
)