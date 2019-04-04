set(PANEL_SOURCES ${PANEL_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/RightPanelWidget.cpp")
set(PANEL_SOURCES ${PANEL_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/CentralPanelWidget.cpp")

target_sources(SpriteExtractor PRIVATE ${PANEL_SOURCES})