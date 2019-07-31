macro(folder_symlink target folder)
    add_custom_command(TARGET ${target} POST_BUILD COMMENT "Creating ${folder} link" COMMAND ${CMAKE_COMMAND} ARGS -E create_symlink "${CMAKE_SOURCE_DIR}/${folder}" "${CMAKE_CURRENT_BINARY_DIR}/${folder}")
endmacro(folder_symlink)
