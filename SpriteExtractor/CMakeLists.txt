cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)

# Add source to this project's executable.
#add_executable(SpriteExtractor WIN32 "main.cpp")
add_executable(SpriteExtractor "main.cpp")
target_sources(SpriteExtractor PRIVATE "App.cpp" "SFMLApp.cpp" "SpriteExtractor.cpp" "imgui-extra.cpp")

if(APPLE)
  set(BUILD_SHARED_LIBS TRUE)
  set_source_files_properties(${PLATFORM_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
  set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-ObjC")
endif(APPLE)

if(WIN32)
  set_target_properties(SpriteExtractor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

include_directories(${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR})

find_package(Freetype REQUIRED)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
if(SFML_FOUND)
    target_link_libraries(SpriteExtractor sfml-graphics sfml-window sfml-system)
endif()

find_package(imgui REQUIRED)
target_link_libraries(SpriteExtractor imgui::imgui)

find_package(OpenGL REQUIRED)

find_package(Boost REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(SpriteExtractor ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})

include(imgui-sfml/CmakeLists.txt)
include(Platform/CmakeLists.txt)
include(Serializers/CMakeLists.txt)
include(Widgets/CMakeLists.txt)
