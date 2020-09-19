set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(tinyobjloader)

set_target_properties(glad glfw tinyobjloader uninstall PROPERTIES FOLDER Vendor)

if(EXISTS ${CMAKE_SOURCE_DIR}/vendor/sponza/)
    file(COPY ${CMAKE_SOURCE_DIR}/vendor/sponza/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res/Global/sponza)
endif()
if(EXISTS ${CMAKE_SOURCE_DIR}/vendor/bunny/)
    file(COPY ${CMAKE_SOURCE_DIR}/vendor/bunny/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res/Global/model/)
endif()
