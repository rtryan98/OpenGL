# build a single example
function(BuildSingleExample EXAMPLE_NAME)
	set(EXAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME})
	message(STATUS "Currently generating example in folder ${EXAMPLE_FOLDER}")
	file(GLOB SOURCE ${EXAMPLE_FOLDER}/src/*.cpp ${EXAMPLE_FOLDER}/src/*.h)
	add_executable(${EXAMPLE_NAME} ${SOURCE})
    target_link_libraries(${EXAMPLE_NAME} Base)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/base/src/)
    if ( MSVC )
        set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
    endif ( MSVC )
    # Copy data
    if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
        if(EXISTS ${EXAMPLE_FOLDER}/res/)
            file(COPY ${EXAMPLE_FOLDER}/res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res/${EXAMPLE_NAME}/)
        endif()
    endif()
    # Filters for Visual Studio
    foreach(item IN ITEMS ${SOURCE})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE "/" "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
    endforeach()
endfunction(BuildSingleExample)

# list all examples
set(EXAMPLES
	Bloom 
)

# build all examples
function(BuildAllExamples)
	foreach(EXAMPLE ${EXAMPLES})
        BuildSingleExample(${EXAMPLE})
	endforeach(EXAMPLE)
endfunction(BuildAllExamples)

BuildAllExamples()
