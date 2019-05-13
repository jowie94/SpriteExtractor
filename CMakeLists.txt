cmake_minimum_required (VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)

project ("SpriteExtractor")

# Include sub-projects.
add_subdirectory("SpriteExtractorLib")
add_subdirectory("lib")
add_subdirectory("SpriteExtractorApp")
add_subdirectory("UIFramework")
