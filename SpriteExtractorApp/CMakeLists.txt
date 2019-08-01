#add_executable(SpriteExtractor WIN32 "main.cpp")
add_executable(SpriteExtractor "main.cpp")
target_sources(SpriteExtractor PRIVATE "App.cpp" "SFMLApp.cpp" "imgui-extra.cpp")
target_include_directories(SpriteExtractor PRIVATE .)

add_subdirectory(imgui-sfml)
add_subdirectory(Platform)
add_subdirectory(Serializers)
add_subdirectory(Windows)
add_subdirectory(Logger)
add_subdirectory(Model)
add_subdirectory(CommandQueue)
add_subdirectory(Services)
add_subdirectory(AssetTypes)

if(APPLE)
    set(BUILD_SHARED_LIBS TRUE)
    set_source_files_properties(${PLATFORM_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-ObjC")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_link_libraries(SpriteExtractor stdc++fs)
    endif()

    add_definitions(-DGL_SILENCE_DEPRECATION)
endif(APPLE)

if(WIN32)
  set_target_properties(SpriteExtractor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

find_package(Freetype REQUIRED)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
if(SFML_FOUND)
    target_link_libraries(SpriteExtractor sfml-graphics sfml-window sfml-system)
endif()

target_link_libraries(SpriteExtractor ImGui)
target_link_libraries(SpriteExtractor UIFramework)

find_package(OpenGL REQUIRED)

find_package(Boost REQUIRED)

add_definitions(-DFMT_HEADER_ONLY)
find_package(spdlog REQUIRED)
target_link_libraries(SpriteExtractor spdlog::spdlog)

target_include_directories(SpriteExtractor PRIVATE ${OPENGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(SpriteExtractor ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})

target_link_libraries(SpriteExtractor SpriteExtractorLib)

include(LinkResources)
folder_symlink(SpriteExtractor "resources")
