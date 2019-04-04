cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)

project ("SpriteExtractor")

# Include sub-projects.
add_subdirectory("SpriteExtractorLib")
add_subdirectory("lib")
add_subdirectory("SpriteExtractorApp")
add_subdirectory("UIFramework")
