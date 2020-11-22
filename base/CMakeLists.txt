file(GLOB_RECURSE BASE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE BASE_H "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_library(Base STATIC ${BASE_SRC} ${BASE_H})

set_target_properties(Base PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(Base PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/bin/)
set_target_properties(Base PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/bin/)

if ( MSVC )
    set_target_properties(Base PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin/)
endif ( MSVC )

target_link_libraries(Base glad glfw tinyobjloader)
target_include_directories(Base PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glm/)

# Filters for Visual Studio
foreach(item IN ITEMS ${BASE_SRC})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE "/" "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
endforeach()
foreach(item IN ITEMS ${BASE_H})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE "/" "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
endforeach()
