/*
 * SceneVisitor.h
 *
 *  Created on: Jan 18, 2010
 *      Author: token
 */

#ifndef SCENEVISITOR_H_
#define SCENEVISITOR_H_

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <iostream>
#include "DynamicNodeDetector.h"
#include <stack>
#include "Mesh.h"
class SceneVisitor: public DynamicNodeDetector
{
public:

    SceneVisitor(std::vector<Mesh>& meshes);
    virtual ~SceneVisitor();

    virtual void apply(osg::Geode& geode);
    virtual void apply(osg::LightSource& light);
    virtual void apply(osg::Group& node);
    virtual void apply(osg::Transform& node);


       const std::vector<osg::Light*>& GetLights() const
        {
            return _lights;
       }

private:
    void processGeometry(osg::Geometry* geo, osg::Matrix localToWorld);

    //		void processGeometry(osg::Geometry* geo, osg::Matrix l_to_w, Material mat);
    std::string storeTexture(osg::Texture2D* tex);
    std::vector<osg::Light*> _lights;
    std::vector<Mesh>& _meshes;
    std::stack<Mesh*> _meshStack;
    //boost::shared_ptr<TextureAtlas> textureAtlas;
};


#endif /* SCENEVISITOR_H_ */
