# CMakeList.txt : CMake project for Rogue-C, include source and define
# project specific logic here.
#

# Add source to this project's executable.

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

add_executable (Rogue-C "Rogue-C.cpp" "Rogue-C.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Rogue-C PROPERTY CXX_STANDARD 20)
endif()
# Specify the path to the raylib headers
include_directories(${CMAKE_SOURCE_DIR}/Rogue-C/include) # Assuming headers are in the include directory

# Specify the path to the raylib import library and DLL
set(RAYLIB_LIB_PATH ${CMAKE_SOURCE_DIR}/Rogue-C/lib) # Assuming raylibdll.lib and raylib.dll are in the lib directory

# Link the raylib import library
target_link_libraries(Rogue-C PRIVATE ${RAYLIB_LIB_PATH}/raylibdll.lib)

# Suppress default libraries if needed
if (MSVC)
    target_link_options(Rogue-C PRIVATE "/NODEFAULTLIB:MSVCRT")
endif()

# Copy the DLL to the build directory
add_custom_command(TARGET Rogue-C POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${RAYLIB_LIB_PATH}/raylib.dll $<TARGET_FILE_DIR:Rogue-C>)