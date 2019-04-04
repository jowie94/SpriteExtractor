add_subdirectory(Popups)
add_subdirectory(Panels)

target_sources(SpriteExtractor PRIVATE "${CMAKE_CURRENT_LIST_DIR}/MainWindowWidget.cpp")
