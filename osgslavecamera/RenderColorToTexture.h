#pragma once
#include <osg/Camera>
#include <osg/Texture2D>

class RenderColorToTexture : public osg::Camera
{
public:
    RenderColorToTexture();
    virtual ~RenderColorToTexture(){}

    osg::Texture2D* getTexture()const
    {
        return _texture.get();
    }

private:
    osg::ref_ptr<osg::Texture2D> _texture;
};
