# build a single example
function(BuildSingleExample EXAMPLE_NAME)
	set(EXAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME})
	message(STATUS "Currently generating example in folder ${EXAMPLE_FOLDER}")
    file(GLOB SOURCE ${EXAMPLE_FOLDER}/src/*.cpp ${EXAMPLE_FOLDER}/src/*.h)
    file(GLOB_RECURSE RES ${EXAMPLE_FOLDER}/res/*.*)
	add_executable(${EXAMPLE_NAME} ${SOURCE} ${RES})
    target_link_libraries(${EXAMPLE_NAME} Base)
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/base/src/)
    if ( MSVC )
    set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
    endif ( MSVC )
    # Filters for Visual Studio
    foreach(item IN ITEMS ${SOURCE})
        get_filename_component(src_path "${item}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
        string(REPLACE "/" "\\" group_path "${group_path}")
        source_group("${group_path}" FILES "${item}")
    endforeach()
    foreach(item IN ITEMS ${RES})
        get_filename_component(src_path "${item}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
        string(REPLACE "/" "\\" group_path "${group_path}")
        source_group("${group_path}" FILES "${item}")
    endforeach()

    # set_target_properties(${EXAMPLE_NAME} PROPERTIES FOLDER API)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/examples)
endfunction(BuildSingleExample)

# list all examples
set(EXAMPLES
    Framebuffers 
    Mandelbulb 
    Models 
    FFT-Ocean
)

# build all examples
function(BuildAllExamples)
	foreach(EXAMPLE ${EXAMPLES})
        BuildSingleExample(${EXAMPLE})
	endforeach(EXAMPLE)
endfunction(BuildAllExamples)

BuildAllExamples()
