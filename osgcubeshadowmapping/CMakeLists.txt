cmake_minimum_required(VERSION 2.8)
project(osgcubeshadowmapping)
include(source_group)
include_directories(..)
add_source_group("Shaders"
	${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/CubeShadowMapVS.glsl
	${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/CubeShadowMapFS.glsl
	${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/CubeShadowMapGS.glsl

	${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/RenderShadowsVS.glsl
	${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/RenderShadowsFS.glsl
        ${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/PCF.glsl
        ${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/PCSS.glsl
        ${osgdemos_DATA_PATH}/shaders/cubeshadowmapping/Hard.glsl
)
add_executable(osgcubeshadowmapping 
	main.cpp
	event_handler.hpp
	event_handler.cpp
	CubeShadowMapping.h
	CubeShadowMapping.cpp
	CubeShadowMap.h
	CubeShadowMap.cpp
	${SOURCE_FILES}
)


target_link_libraries(osgcubeshadowmapping common ${OPENGL_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} )
