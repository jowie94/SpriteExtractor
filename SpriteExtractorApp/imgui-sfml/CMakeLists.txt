add_definitions(-DIMGUI_USER_CONFIG=\"custom-imconfig.h\")

target_sources(SpriteExtractor PRIVATE "${CMAKE_CURRENT_LIST_DIR}/imgui-SFML.cpp")
#target_include_directories(SpriteExtractor PRIVATE "imgui-sfml")