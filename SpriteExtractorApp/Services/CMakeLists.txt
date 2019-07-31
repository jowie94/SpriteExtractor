add_subdirectory(AssetManager)

set(SERVICES_SOURCES ${SERVICES_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/Services.cpp")

target_sources(SpriteExtractor PRIVATE ${SERVICES_SOURCES})
