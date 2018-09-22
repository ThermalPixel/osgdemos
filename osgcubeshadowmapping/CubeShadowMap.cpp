#include "CubeShadowMap.h"

#include <common/Utils.h>
#include "DataPath.h"


CubeShadowMap::CubeShadowMap(int size, const osg::Vec3& lightPos)
    : _lightPos(lightPos), _farPlaneDist(100.0f),_nearPlaneDist(0.1f)
{
	setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    setName("CubeMap");
	_shadowMap = new osg::TextureCubeMap;
	_shadowMap->setTextureSize(size,size);
    _shadowMap->setInternalFormat(GL_DEPTH_COMPONENT);
    //_shadowMap->setInternalFormat(GL_RGB);

	_shadowMap->setSourceType(GL_FLOAT);
	_shadowMap->setSourceFormat(GL_DEPTH_COMPONENT);
	_shadowMap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    _shadowMap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    _shadowMap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
    _shadowMap->setFilter(osg::TextureCubeMap::MIN_FILTER,osg::TextureCubeMap::LINEAR);
    _shadowMap->setFilter(osg::TextureCubeMap::MAG_FILTER,osg::TextureCubeMap::LINEAR);
	setClearMask( GL_DEPTH_BUFFER_BIT   );
	setClearColor(osg::Vec4(1,1,1,1));
	setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	setRenderOrder(osg::Camera::PRE_RENDER);
	setCullingActive(false);
	setCullingMode(osg::CullSettings::NO_CULLING);
	setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
	attach(osg::Camera::DEPTH_BUFFER, _shadowMap,0,osg::Camera::FACE_CONTROLLED_BY_GEOMETRY_SHADER);;
	setImplicitBufferAttachmentMask(osg::Camera::IMPLICIT_DEPTH_BUFFER_ATTACHMENT );

	setViewport(0,0,size,size);
	
	setProjectionMatrix(osg::Matrix::identity());
	setViewMatrix(osg::Matrix::identity());
	


	
	
	
	osg::Shader * vs = new osg::Shader(osg::Shader::VERTEX);
    vs->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/CubeShadowMapVS.glsl");
	
	osg::Shader * gs = new osg::Shader(osg::Shader::GEOMETRY);
    gs->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/CubeShadowMapGS.glsl");
	
	osg::Shader * fs = new osg::Shader(osg::Shader::FRAGMENT);
    fs->loadShaderSourceFromFile(DataPath + "/shaders/cubeshadowmapping/CubeShadowMapFS.glsl");
	_program = new osg::Program();


	_program->addShader(vs);
	_program->addShader(fs);
	_program->addShader(gs);
	_program->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 18 );
	_program->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES );
	_program->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP );

	getOrCreateStateSet()->setAttributeAndModes(_program, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	//getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE ); 
    _matUniform = new  osg::Uniform(osg::Uniform::FLOAT_MAT4, "cubeMapMatrices",6);

    getOrCreateStateSet()->addUniform(_matUniform);

    getOrCreateStateSet()->addUniform(new osg::Uniform("far",_farPlaneDist));
	getOrCreateStateSet()->addUniform(new osg::Uniform("lpos",_lightPos));

    getOrCreateStateSet()->addUniform(new osg::Uniform("diffuseTex",0));
    //getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );

}

void CubeShadowMap::traverse(osg::NodeVisitor &nv)
{
    if(nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR) {
        typedef std::pair<osg::Vec3, osg::Vec3> ImageData;
        const ImageData id[] =
        {
            ImageData( osg::Vec3( 1,  0,  0), osg::Vec3( 0, -1,  0) ), // +X
            ImageData( osg::Vec3(-1,  0,  0), osg::Vec3( 0, -1,  0) ), // -X
            ImageData( osg::Vec3( 0,  1,  0), osg::Vec3( 0,  0,  1) ), // +Y
            ImageData( osg::Vec3( 0, -1,  0), osg::Vec3( 0,  0, -1) ), // -Y
            ImageData( osg::Vec3( 0,  0,  1), osg::Vec3( 0, -1,  0) ), // +Z
            ImageData( osg::Vec3( 0,  0, -1), osg::Vec3( 0, -1,  0) )  // -Z
        };
        osg::Matrixf proj;
        proj.makePerspective(90.0f,1.0f,_nearPlaneDist,_farPlaneDist);
        for (int i =0; i < 6;++i)
        {
            osg::Matrixf viewMatrix ;
            viewMatrix.makeLookAt(_lightPos,_lightPos + id[i].first, id[i].second);
            viewMatrix =  viewMatrix * proj;
            _matUniform->setElement(i, viewMatrix);
        }
        getOrCreateStateSet()->getUniform("lpos")->set(_lightPos);
    }
    osg::Camera::traverse(nv);

}
