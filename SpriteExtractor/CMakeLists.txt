# CMakeList.txt : CMake project for SpriteExtractor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CIMG_H_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 14)

# Add source to this project's executable.
add_executable (SpriteExtractor "SpriteExtractor.cpp")

if(NOT WIN32)
  #FIND_PACKAGE(X11)
  add_definitions(-Dcimg_display=0)
  FIND_PACKAGE(Threads REQUIRED)
endif()

if(NOT APPLE)
  if(NOT WIN32)
    if(X11_FOUND)
      SET(CIMG_CFLAGS "${CIMG_CFLAGS} ${CIMG_XSHM_CCFLAGS} ${CIMG_XRANDR_CCFLAGS}")
      SET(SYSTEM_LIBS ${SYSTEM_LIBS} Xext Xrandr)
    endif()
  endif(NOT WIN32)
endif(NOT APPLE)

if(X11_FOUND)
  link_directories(${X11_LIB_DIRS})
  include_directories(${X11_INCLUDE_DIR})
  SET( SYSTEM_LIBS ${SYSTEM_LIBS} ${X11_LIBRARIES} )
endif()

if (NOT WIN32)
  SET( SYSTEM_LIBS ${SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
endif()

if(APPLE)
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "Platform/MacPlatform.mm")
  set(BUILD_SHARED_LIBS TRUE)
  set_source_files_properties(${PLATFORM_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif(APPLE)

if(WIN32)
  SET( SYSTEM_LIBS  ${SYSTEM_LIBS}  gdi32)
  set_target_properties(SpriteExtractor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  set(PLATFORM_SOURCES ${PLATFORM_SOURCES} "Platform/WindowsPlatform.cpp")
endif()

target_sources(SpriteExtractor PRIVATE ${PLATFORM_SOURCES})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_CFLAGS}")

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
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(SpriteExtractor ${OPENGL_LIBRARIES})

target_link_libraries(SpriteExtractor ${SYSTEM_LIBS})

# TODO: Add tests and install targets if needed.
