if (WIN32)
    if (MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    endif()
endif()

add_executable (Rogue-C "Rogue-C.cpp" "Rogue-C.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Rogue-C PROPERTY CXX_STANDARD 20)
endif()

if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/Rogue-C/include)

    set(RAYLIB_LIB_PATH ${CMAKE_SOURCE_DIR}/Rogue-C/lib/win64)

    target_link_libraries(Rogue-C PRIVATE ${RAYLIB_LIB_PATH}/raylibdll.lib)

    if (MSVC)
        target_link_options(Rogue-C PRIVATE "/NODEFAULTLIB:MSVCRT")
    endif()

    add_custom_command(TARGET Rogue-C POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RAYLIB_LIB_PATH}/raylib.dll $<TARGET_FILE_DIR:Rogue-C>)

elseif(UNIX)
    set(RAYLIB_LIB_PATH ${CMAKE_SOURCE_DIR}/Rogue-C/lib/linux64)

    target_link_libraries(Rogue-C PRIVATE ${RAYLIB_LIB_PATH}/libraylib.a)
endif()