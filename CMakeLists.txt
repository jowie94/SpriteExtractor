cmake_minimum_required (VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project ("SpriteExtractor")

set(CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/CMake"
)

# Include sub-projects.
add_subdirectory("SpriteExtractorLib")
add_subdirectory("lib")
add_subdirectory("SpriteExtractorApp")
add_subdirectory("UIFramework")
