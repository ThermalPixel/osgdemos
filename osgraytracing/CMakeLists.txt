cmake_minimum_required(VERSION 2.8)
project(osgraytracing)
include(source_group)
include_directories(.. ../external/embree/include)

add_executable(osgraytracing 
	main.cpp
	EmbreeTypedefs.h

	DynamicNodeDetector.h
	SceneVisitor.h
	SceneVisitor.cpp
	TransformReadVisitor.h
	TransformReadVisitor.cpp
	TriangleIndexWriter.h
	TriangleIndexWriter.cpp
	VecVisitor.h
	Mesh.h
	EventHandler.h
	EventHandler.cpp
	Raytracer.h
	Raytracer.cpp
)


target_link_libraries(osgraytracing common ${OPENGL_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} embree${EXT})
