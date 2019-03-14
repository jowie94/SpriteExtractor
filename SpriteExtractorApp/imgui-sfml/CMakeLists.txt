target_compile_definitions(SpriteExtractor PRIVATE -DIMGUI_USER_CONFIG=\"custom-imconfig.h\")

target_sources(SpriteExtractor PRIVATE "${CMAKE_CURRENT_LIST_DIR}/imgui-SFML.cpp")
