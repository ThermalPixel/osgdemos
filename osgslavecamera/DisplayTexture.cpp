#include "DisplayTexture.h"
#include <osg/Geometry>
#include <osg/Geode>
osg::Geode * createFullscreenQuad() {
     osg::Geometry* geom = osg::createTexturedQuadGeometry(osg::Vec3(), osg::Vec3(1, 0.0f, 0.0f), osg::Vec3(0.0f, 1, 0.0f), 0.0f, 0.0f, 1.0f, 1.0f);
     osg::Geode *quad = new osg::Geode;
     quad->addDrawable(geom);
     return quad;
 }


DisplayTexture::DisplayTexture()
{
    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    setRenderOrder(osg::Camera::POST_RENDER);
    setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setViewMatrix(osg::Matrix());
    setProjectionMatrix(osg::Matrix::ortho2D(0, 1, 0, 1));
    getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF);
    addChild(createFullscreenQuad());

    setName("Display");

}

void DisplayTexture::setTexture(osg::Texture2D *tex)
{
     getOrCreateStateSet()->setTextureAttributeAndModes(0, tex,osg::StateAttribute::ON);
}
