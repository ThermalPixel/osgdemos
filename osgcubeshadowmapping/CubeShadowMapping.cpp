#include "CubeShadowMapping.h"
#include <common/Utils.h>
#include "DataPath.h"

CubeShadowMapping::CubeShadowMapping(CubeShadowMap * cubemap)
	: _cs(cubemap)
{
    setReferenceFrame( osg::Transform::RELATIVE_RF );
    setRenderOrder(osg::Camera::POST_RENDER);
    setName("Shadows");
	getOrCreateStateSet()->setTextureAttributeAndModes(5, cubemap->getCubeMap(), osg::StateAttribute::ON);
	_program = new osg::Program();
	osg::Shader * vs = new osg::Shader(osg::Shader::VERTEX);
    vs->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/RenderShadowsVS.glsl");

	osg::Shader * fs = new osg::Shader(osg::Shader::FRAGMENT);
    fs->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/RenderShadowsFS.glsl");

	osg::Shader * shadow = new osg::Shader(osg::Shader::FRAGMENT);
    shadow->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/PCSS.glsl");

	_program->addShader(shadow);
	_program->addShader(vs);
	_program->addShader(fs);
	

	getOrCreateStateSet()->setAttributeAndModes(_program, osg::StateAttribute::ON);
	getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	getOrCreateStateSet()->addUniform(new osg::Uniform("cubeMap",5));
	getOrCreateStateSet()->addUniform(new osg::Uniform("lpos",cubemap->getLightPosition()));
    getOrCreateStateSet()->addUniform(new osg::Uniform("far",cubemap->getFarPlaneDist()));
	getOrCreateStateSet()->addUniform(new osg::Uniform("diffuseTex",0));
    getOrCreateStateSet()->addUniform(new osg::Uniform("w_light",0.1f));
    getOrCreateStateSet()->addUniform(new osg::Uniform("near",cubemap->getNearPlaneDist()));
}

void CubeShadowMapping::traverse(osg::NodeVisitor& nv)
{
    if(nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR) {
        getOrCreateStateSet()->getUniform("lpos")->set(_cs->getLightPosition());
    }
    osg::Camera::traverse(nv);
}
