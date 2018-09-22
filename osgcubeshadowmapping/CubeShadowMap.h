#pragma once
#include <osg/Camera>
#include <osg/TextureCubeMap>
class CubeShadowMap : public osg::Camera
{
public:
    CubeShadowMap(int size, const osg::Vec3& lightPos);
	osg::TextureCubeMap * getCubeMap() const { return _shadowMap;}
	osg::Vec3f getLightPosition() const
	{
		return _lightPos;
	}

    float getFarPlaneDist() const
    {
        return _farPlaneDist;
    }
    float getNearPlaneDist() const
    {
        return _nearPlaneDist;
    }
    void setLightPos(const osg::Vec3& lightPos)
    {
        _lightPos =lightPos;
    }
    virtual void traverse(osg::NodeVisitor &nv);
private:
	osg::ref_ptr<osg::TextureCubeMap> _shadowMap;
	osg::ref_ptr<osg::Program> _program;
	osg::Vec3f _lightPos;
    float _farPlaneDist;
    float _nearPlaneDist;
    osg::Uniform * _matUniform;

};
