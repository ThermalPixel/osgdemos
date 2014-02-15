cmake_minimum_required(VERSION 2.8)
project(osgdemos)

find_package(OpenGL)
if(OPENGL_FOUND)
  include_directories(  
    ${OPENGL_INCLUDE_DIR} 
  )
  link_directories(
    ${OPENGL_LIBRARY_DIRS}
  )   
endif()

find_package(OpenSceneGraph COMPONENTS osgDB osgViewer osgGA osgUtil REQUIRED)

include_directories( ${OPENSCENEGRAPH_INCLUDE_DIRS})

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x")
if( CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]" )
add_definitions("-DDEBUG")
endif()


add_subdirectory(osgdebug)
add_subdirectory(osgslavecamera)