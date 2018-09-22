#pragma once

#include "CubeShadowMap.h"
class CubeShadowMapping : public osg::Camera
{
public:
	CubeShadowMapping(CubeShadowMap * cs);
    virtual void traverse(osg::NodeVisitor& nv);
private:
	osg::ref_ptr<CubeShadowMap> _cs;
	osg::ref_ptr<osg::Program> _program;

};
