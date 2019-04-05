set(PANEL_SOURCES ${PANEL_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/SearchConfigPanel.cpp")
set(PANEL_SOURCES ${PANEL_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/SpritesPanel.cpp")

target_sources(SpriteExtractor PRIVATE ${PANEL_SOURCES})