if(APPLE)
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/MacPlatform.mm")
endif(APPLE)

if(WIN32)
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "${CMAKE_CURRENT_LIST_DIR}/WindowsPlatform.cpp")
endif()

target_sources(SpriteExtractor PRIVATE ${PLATFORM_SOURCES})
