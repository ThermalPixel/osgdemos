project(osgslavecamera)
set(SOURCE_FILES
	main.cpp
	RenderColorToTexture.cpp
	RenderColorToTexture.h
	DisplayTexture.h
	DisplayTexture.cpp
)
add_executable(osgslavecamera ${SOURCE_FILES})
target_link_libraries(osgslavecamera  ${OPENGL_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} )
