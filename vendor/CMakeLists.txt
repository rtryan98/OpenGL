set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(tinyobjloader)

set_target_properties(glad glfw tinyobjloader uninstall PROPERTIES FOLDER Vendor)
