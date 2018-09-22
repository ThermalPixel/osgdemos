/*
 * SceneVisitor.cpp
 *
 *  Created on: Jan 18, 2010
 *      Author: token
 */

#include "SceneVisitor.h"

#include "TriangleIndexWriter.h"

#include <string.h>
#include <osg/Geometry>

#include <osg/LightSource>
#include <osg/io_utils>
#include <osgSim/DOFTransform>
using namespace std;

    SceneVisitor::SceneVisitor(std::vector<Mesh> &meshes)
        :_meshes(meshes)
	{

        //add the static mesh id
        _meshes.push_back(Mesh());
		//push the static mesh
        _meshStack.push(&_meshes[_meshes.size() - 1]);
/*        textureAtlas = atlas;*/
	}

	SceneVisitor::~SceneVisitor()
	{

	}
	void SceneVisitor::apply(osg::LightSource& lightNode)
	{
        _lights.push_back(lightNode.getLight());
    /*	Light rtLight;
		osg::Light* light = lightNode.getLight();
		//directional light
		rtLight.position = osg::Vec4f(light->getPosition().x(), light->getPosition().y(), light->getPosition().z(), 1);
		if (light->getPosition() == osg::Vec4f(0, 0, 0, 1))
		{
			rtLight.position = osg::Vec4f(light->getDirection(), 0);
		}
		rtLight.ambient = osg::Vec3f(light->getAmbient().x(), light->getAmbient().y(), light->getAmbient().z());
		rtLight.diffuse = osg::Vec3f(light->getDiffuse().x(), light->getDiffuse().y(), light->getDiffuse().z());
		rtLight.specular = osg::Vec3f(light->getSpecular().x(), light->getSpecular().y(), light->getSpecular().z());
		//encode the attenuation as vec3
		rtLight.attenuation = osg::Vec3f(light->getConstantAttenuation(), light->getLinearAttenuation(),
				light->getQuadraticAttenuation());
		//add light
        lights.push_back(rtLight);*/
	}
	void SceneVisitor::apply(osg::Transform& node)
	{
       bool dynamicNode = isDynamicNode(node);
		if (dynamicNode)
		{
            _meshes.push_back(Mesh());
            _meshStack.push(&_meshes[_meshes.size() - 1]);
		}
		traverse(node);
		if (dynamicNode)
            _meshStack.pop();
	}

	void SceneVisitor::apply(osg::Group& group)
	{
		traverse(group);
	}
	void SceneVisitor::apply(osg::Geode& geode)
	{
/*
		Material mat;

		osg::StateSet* stset = geode.getStateSet();
		if (stset)
		{

			osg::StateAttribute* attr = stset->getTextureAttribute(0, osg::StateAttribute::TEXTURE);
			osg::Texture2D* texture2D = 0;
			unsigned int mat_id = 0;

			if (attr)
			{
				texture2D = dynamic_cast<osg::Texture2D*> (attr);
				osg::Material* material = dynamic_cast<osg::Material*> (stset->getAttribute(
						osg::StateAttribute::MATERIAL, 0));

				if (material)
				{
					mat.diffuse = material->getDiffuse(osg::Material::FRONT);
					mat.specular = material->getSpecular(osg::Material::FRONT);
				} else
				{
					mat.diffuse = SV_CONFIG::default_diffuse;
					mat.specular = SV_CONFIG::default_specular;
				}

				if (texture2D)
				{
					mat.hasTexture = true;
					mat.texName = storeTexture(texture2D);
					if (mat.texName == "")
						mat.hasTexture = false;
				}
			}
		} else
		{
			// No StateSet found in Geode, hopefully there will be one in the geometry
		}
*/
        osg::Matrix localToWorld = osg::computeLocalToWorld(getNodePath());

		//	if( m.isIdentity())
		//		cout << "Geode [" << geode.getName() << "] has identity local to world" << endl;
		//	else
		//		cout << "Geode [" << geode.getName() << "] has CUSTOM local to world" << endl;

		unsigned int count = geode.getNumDrawables();

		for (unsigned int i = 0; i < count; ++i)
		{
			osg::Geometry* geo = geode.getDrawable(i)->asGeometry();
			if (geo != NULL)
			{
                //if (stset)
//					processGeometry(geo, localToWorld, mat);
    //			else
                    processGeometry(geo,localToWorld);
			}
        }
	}

/*void SceneVisitor::processGeometry(osg::Geometry* geo, osg::Matrix l_to_w, Material mat)
	{

        for (unsigned int i = 0; i < geo->getNumPrimitiveSets(); ++i)
		{
			osg::PrimitiveSet* ps = geo->getPrimitiveSet(i);
			if (meshStack.size() > 1)
			{
				//	l_to_w = osg::Matrix::identity();
			}
			TriangleIndexWriter tiw(geo, meshStack.top(), l_to_w, mat, textureAtlas.get());
			ps->accept(tiw);
        }
	}
*/
    void SceneVisitor::processGeometry(osg::Geometry* geo, osg::Matrix localToWorld)
	{
  /*      for (int i =0; i < geo->getVertexArray()->getNumElements();++i)
        {
            Vec3Visitor visitor;
            geo->getVertexArray()->accept(i,visitor);
            osg::Vec4f vec;
            vec.x() = visitor.getValue().x();
              vec.y() = visitor.getValue().y();
                vec.z() = visitor.getValue().z();

            _meshStack.top()->addVertex(vec );
        }
*/
        for (unsigned int i = 0; i < geo->getNumPrimitiveSets(); ++i)
		{
            osg::PrimitiveSet* ps = geo->getPrimitiveSet(i);
            TriangleIndexWriter tiw(geo, _meshStack.top(), localToWorld);
			ps->accept(tiw);
        }
	}

	std::string SceneVisitor::storeTexture(osg::Texture2D* tex)
	{
/*
		if (textureAtlas && tex && tex->getImage(0))
		{
			return textureAtlas->AddTexture(tex->getImage(0), osg::Vec2f(tex->getImage(0)->s(), tex->getImage(0)->t()));
		}
        return "";*/

	}
