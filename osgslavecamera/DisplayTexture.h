#pragma once
#include <osg/Camera>
#include <osg/Texture2D>

class DisplayTexture : public osg::Camera
{
public:
    DisplayTexture();

    virtual ~DisplayTexture(){}

    void setTexture(osg::Texture2D * tex);

};
