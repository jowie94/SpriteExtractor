# CMakeList.txt : CMake project for SpriteExtractor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CIMG_H_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 14)

# Add source to this project's executable.
#add_executable(SpriteExtractor WIN32 "main.cpp" "SpriteExtractor.cpp" "Serializers/Paper2DSerializer.cpp")
add_executable(SpriteExtractor "main.cpp" "SpriteExtractor.cpp" "Serializers/Paper2DSerializer.cpp")
target_sources(SpriteExtractor PRIVATE "App.cpp" "SFMLApp.cpp")

if(APPLE)
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "Platform/MacPlatform.mm")
  set(BUILD_SHARED_LIBS TRUE)
  set_source_files_properties(${PLATFORM_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
  set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-ObjC")
endif(APPLE)

if(WIN32)
  set_target_properties(SpriteExtractor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "Platform/WindowsPlatform.cpp")
endif()

target_sources(SpriteExtractor PRIVATE ${PLATFORM_SOURCES})

include_directories( ${PROJECT_SOURCE_DIR} )

add_definitions(-DIMGUI_USER_CONFIG=\"custom-imconfig.h\")

target_sources(SpriteExtractor PRIVATE "imgui-sfml/imgui-SFML.cpp")
target_include_directories(SpriteExtractor PRIVATE . "imgui-sfml")

find_package(Freetype REQUIRED)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
if(SFML_FOUND)
    target_link_libraries(SpriteExtractor sfml-graphics sfml-window sfml-system)
endif()

find_package(imgui REQUIRED)
target_link_libraries(SpriteExtractor imgui::imgui )

find_package(OpenGL REQUIRED)

find_package(Boost REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(SpriteExtractor ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})

# TODO: Add tests and install targets if needed.
