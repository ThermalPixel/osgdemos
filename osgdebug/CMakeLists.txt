project(osgdebug)
set(SOURCE_FILES
	main.cpp
	EnableGLDebugOperation.cpp
	EnableGLDebugOperation.h
)
add_executable(osgdebug ${SOURCE_FILES})
target_link_libraries(osgdebug  ${OPENGL_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} )
